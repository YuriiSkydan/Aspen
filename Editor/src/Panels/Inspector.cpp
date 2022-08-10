#include "Inspector.h"
#include "src/ScriptManager.h"
#include "imgui/imgui_stdlib.h"

using namespace std::string_literals;

Inspector::Inspector(Ptr<GameObject>& gameObjectRef)
	:m_SelectedGameObject(gameObjectRef)
{ }

void Inspector::ImGuiRender()
{
	ImGui::Begin("Inspector");

	if (m_TagsAndLayersManager)
	{
		RenderTagsAndLayersManager();
	}
	else if (m_SelectedGameObject != nullptr)
	{
		RenderGameObject();
	}

	ImGui::End();
}

void Inspector::RenderMaterial(unsigned int collumnIndex, Material* material)
{
	switch (collumnIndex)
	{
	case 0:
		ImGui::Text("Material");
		ImGui::Spacing();
		ImGui::Text("Friction");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Dencity");
		break;
	case 1:
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat("##Friction", &material->friction, 0.01f, 0.0f, FLT_MAX);
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat("##Dencity", &material->dencity, 0.01f, 0.0f, FLT_MAX);
		break;
	}
}

void Inspector::RenderGameObject()
{
	RenderGameObjectProperties();
	RenderComponents();
	RenderAddComponentButton();
}

void Inspector::RenderGameObjectProperties()
{
	bool active = m_SelectedGameObject->IsActive();
	ImGui::Checkbox("##IsActive", &active);
	m_SelectedGameObject->SetActive(active);

	ImGui::SameLine();
	ImGui::InputText("##", m_SelectedGameObject->m_Name, 20);

	ImGui::Columns(2, "##Tags and Layers", false);
	ImGui::SetColumnWidth(0, ImGui::GetWindowSize().x / 2.0f);

	ImGui::Spacing();
	ImGui::Text("Tag");
	ImGui::SameLine();

	std::string tagName = m_SelectedGameObject->GetTag().GetName();
	if (ImGui::BeginCombo("##Tags", tagName.c_str()))
	{
		for (auto& tag : Tag::GetTags())
		{
			if (ImGui::Selectable(tag.c_str()))
			{
				m_SelectedGameObject->SetTag(Tag(tag));
			}
		}

		ImGui::Separator();

		if (ImGui::Selectable("Add Tag"))
			m_TagsAndLayersManager = true;

		ImGui::EndCombo();
	}

	ImGui::NextColumn();
	ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x / 2.0f);

	ImGui::Spacing();
	ImGui::Text("Layer");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Layers", "Default"))
	{
		ImGui::EndCombo();
	}

	ImGui::Columns(1);

	ImGui::Separator();
}

void Inspector::RenderComponents()
{
	m_SecondCollumnWidth = ImGui::GetWindowSize().x - m_FirstCollumnWidth;
	m_ItemWidth = m_SecondCollumnWidth - 15;

	auto& components = m_SelectedGameObject->GetComponents();
	for (auto& it : components)
		RenderComponents<AllComponents>(it.get());

	for (auto& script : m_SelectedGameObject->GetScripts())
	{
		ImGui::CollapsingHeader(script->GetName().c_str());
		ImGui::Separator();
	}

	ImGui::NewLine();
	ImGui::NewLine();
}

bool Inspector::RenderComponentHeader(const std::string& componentName, Component* component, bool isEditable)
{
	bool isOpen = ImGui::CollapsingHeader(("##"s + componentName).c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);
	bool removeComponent = false;
	ImGui::SameLine();

	ImGui::PushID(componentName.c_str());
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		ImGui::OpenPopup("Component properties");

	if (ImGui::BeginPopup("Component properties"))
	{
		if (ImGui::MenuItem("Remove component"))
			removeComponent = true;

		ImGui::EndPopup();
	}
	ImGui::PopID();

	if (isEditable)
	{
		bool isEnabled = component->IsEnabled();
		ImGui::Checkbox(("##Is Enabled"s + componentName).c_str(), &isEnabled);
		component->SetEnabled(isEnabled);
	}

	ImGui::SameLine();
	ImGui::Text(componentName.c_str());

	if (removeComponent)
	{
		m_SelectedGameObject->RemoveComponent(component);
		return false;
	}

	return isOpen;
}

void Inspector::RenderComponent(Transform* transform)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Position");
		ImGui::Spacing();
		ImGui::Text("Rotation");
		ImGui::Spacing();
		ImGui::Text("Scale");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat2("##Position ", (float*)&transform->position, 0.01f);
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat("##Rotation", &transform->angle, 0.01f);
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat2("##Scale  ", (float*)&transform->scale.x, 0.01f);

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(SpriteRenderer* spriteRenderer)
{
	bool isOpen = RenderComponentHeader("SpriteRenderer", spriteRenderer, true);

	if (isOpen)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Order In Layer");
		ImGui::Spacing();
		ImGui::Text("Color");
		ImGui::Spacing();
		ImGui::Text("Flip X");
		ImGui::Spacing();
		ImGui::Text("Flip Y");
		ImGui::Spacing();
		ImGui::Text("Texture");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragInt("##Order In Layer", &spriteRenderer->orderInLayer, 1, 0);

		ImGui::SetNextItemWidth(m_ItemWidth);
		auto color = spriteRenderer->GetColor();
		if (ImGui::ColorEdit4("##Color", (float*)&color))
			spriteRenderer->SetColor(color);

		ImGui::Checkbox("##FlipX", &spriteRenderer->flipX);
		ImGui::Checkbox("##FlipY", &spriteRenderer->flipY);

		float imageButtonSize = m_ItemWidth > 128 ? 128 : m_ItemWidth;
		ImGui::Image((ImTextureID)spriteRenderer->GetTexture()->GetID(), { float(imageButtonSize), float(imageButtonSize) }, { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROJECT_PANEL_ITEM"))
			{
				const char* path = (const char*)payload->Data;
				std::cout << "Path: " << path << std::endl;
				spriteRenderer->SetSprite(path);

				ImGui::EndDragDropTarget();
			}
		}

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(Camera* camera)
{
	bool isOpen = RenderComponentHeader("Camera", camera, true);

	if (isOpen)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Background");
		ImGui::Spacing();
		ImGui::Text("Size"); ImGui::SameLine();

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::ColorEdit4("##Background Color", (float*)&camera->backgroundColor);
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat("##Size", &camera->size, 0.001f);

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(Rigidbody* rigidbody)
{
	bool isOpen = RenderComponentHeader("Rigidbody", rigidbody, false);

	if (isOpen)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Body Type");
		ImGui::Spacing();
		ImGui::Text("Mass");
		ImGui::Spacing();
		ImGui::Text("Gravity Scale");
		ImGui::Spacing();
		ImGui::Text("Linear  Drag");
		ImGui::Spacing();
		ImGui::Text("Angular Drag");

		ImGui::NewLine();
		ImGui::Spacing();

		ImGui::Text("Fixed Rotation");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		std::string bodyTypeStr;
		if (rigidbody->GetBodyType() == BodyType(b2_dynamicBody))
			bodyTypeStr = "Dynamic";
		else if (rigidbody->GetBodyType() == BodyType(b2_staticBody))
			bodyTypeStr = "Static";
		else
			bodyTypeStr = "Kinematic";

		ImGui::SetNextItemWidth(m_ItemWidth);
		if (ImGui::BeginCombo("##BodyType", bodyTypeStr.c_str()))
		{
			if (ImGui::Selectable("Dynamic"))
				rigidbody->SetBodyType(BodyType(b2_dynamicBody));
			if (ImGui::Selectable("Static"))
				rigidbody->SetBodyType(BodyType(b2_staticBody));
			if (ImGui::Selectable("Kinematic"))
				rigidbody->SetBodyType(BodyType(b2_kinematicBody));

			ImGui::EndCombo();
		}

		ImGui::SetNextItemWidth(m_ItemWidth);
		DragFloat("##Mass", rigidbody, rigidbody->GetMass(),
			&Rigidbody::SetMass, 0.01f, 0.0f);

		ImGui::SetNextItemWidth(m_ItemWidth);
		DragFloat("##GravityScale", rigidbody, rigidbody->GetGravityScale(),
			&Rigidbody::SetGravityScale, 0.01f);

		ImGui::SetNextItemWidth(m_ItemWidth);
		DragFloat("##LinearDrag", rigidbody, rigidbody->GetLinearDrag(),
			&Rigidbody::SetLinearDrag, 0.01f);

		ImGui::SetNextItemWidth(m_ItemWidth);
		DragFloat("##AngularDrag", rigidbody, rigidbody->GetAngularDrag(),
			&Rigidbody::SetAngularDrag, 0.01f);

		ImGui::Spacing();
		ImGui::Spacing();

		Checkbox("##FixedRotation", rigidbody, rigidbody->GetFixedRotation(),
			&Rigidbody::SetFixedRotation);

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(BoxCollider* boxCollider)
{
	bool isOpen = RenderComponentHeader("BoxCollider", boxCollider, true);

	if (isOpen)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Size");
		ImGui::Spacing();
		ImGui::Text("Offset");
		ImGui::Spacing();
		ImGui::Text("Is Trigger");
		ImGui::Spacing();

		RenderMaterial(0);

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat2("##Size", (float*)&boxCollider->size, 0.01f, 0.0f, FLT_MAX, "%.3f");
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat2("##Offset", (float*)&boxCollider->offset, 0.01f);
		ImGui::Checkbox("##IsTrigger", &boxCollider->isTrigger);
		ImGui::NewLine();

		RenderMaterial(1, &boxCollider->material);

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(CircleCollider* circleCollider)
{
	bool isOpen = RenderComponentHeader("CircleCollider", circleCollider, true);

	if (isOpen)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Radius");
		ImGui::Spacing();
		ImGui::Text("Offset");
		ImGui::Spacing();
		ImGui::Text("Is Trigger");
		ImGui::Spacing();

		RenderMaterial(0);

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat("##Radius", &circleCollider->radius, 0.001f, 0.0f);
		ImGui::SetNextItemWidth(m_ItemWidth);
		ImGui::DragFloat2("##Offset", (float*)&circleCollider->offset, 0.01f);
		ImGui::Checkbox("##IsTrigger", &circleCollider->isTrigger);
		ImGui::NewLine();

		RenderMaterial(1, &circleCollider->material);

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(PolygonCollider* polygonCollider)
{
	bool isOpen = RenderComponentHeader("PolygonCollider", polygonCollider, true);

	if (isOpen)
	{

	}
}

void Inspector::RenderComponent(Animator* animator)
{
	bool isOpen = RenderComponentHeader("Animator", animator, true);

	if (isOpen)
	{
		ImGui::Text("Add Animation");
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			std::string name;
			if (m_ChoosenClip != nullptr)
				name = m_ChoosenClip->GetName();

			animator->AddAnimation("Animation" + std::to_string(animator->m_AnimationClips.size()));

			if (m_ChoosenClip != nullptr)
				m_ChoosenClip = &animator->GetAnimation(name);
		}

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Text("Animation ");

		if (m_ChoosenClip != nullptr)
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("Name ");
			ImGui::Spacing();
			ImGui::Text("Duration ");
		}

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		std::string clipName = "None";
		if (m_ChoosenClip != nullptr)
			clipName = m_ChoosenClip->GetName();

		ImGui::SetNextItemWidth(m_ItemWidth);
		if (ImGui::BeginCombo("##Animation", clipName.c_str()))
		{
			for (auto& clip : animator->m_AnimationClips)
			{
				if (ImGui::Selectable(clip.GetName().c_str()))
					m_ChoosenClip = &clip;
			}

			ImGui::EndCombo();
		}

		if (m_ChoosenClip != nullptr)
		{
			//m_ChoosenClip->Update();

			ImGui::Spacing();
			ImGui::InputText("##ClipName", &clipName);

			DragFloat("##Duration", m_ChoosenClip, m_ChoosenClip->GetDuration(),
				&AnimationClip::SetDuration, 0.01f, 0.0f);

			ImGui::Spacing();

			float imageSize = m_ItemWidth;
			if (m_ItemWidth > 128)
				imageSize = 128;

			if (m_ChoosenClip->GetFramesAmount() == 0)
			{
				SpriteRenderer* renderer = m_SelectedGameObject->GetComponent<SpriteRenderer>();

				if (renderer != nullptr)
					ImGui::Image((ImTextureID)renderer->GetTexture()->GetID(), { float(imageSize), float(imageSize) }, { 0, 1 }, { 1, 0 });
			}
			else if (m_ChoosenClip->GetFrame() != nullptr)
			{
				m_ChoosenClip->Update();
				ImGui::Image((ImTextureID)m_ChoosenClip->GetFrame()->GetID(), { float(imageSize), float(imageSize) }, { 0, 1 }, { 1, 0 });
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROJECT_PANEL_ITEM"))
				{
					const char* path = (const char*)payload->Data;
					m_ChoosenClip->AddFrame(path);
					m_ChoosenClip->Start();

					ImGui::EndDragDropTarget();
				}
			}
		}

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(AudioSource* audioSource)
{
	bool isOpen = RenderComponentHeader("Audio Source", audioSource, true);

	if (isOpen)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, m_FirstCollumnWidth);

		ImGui::Spacing();
		ImGui::Text("Min Distance");
		ImGui::Spacing();
		ImGui::Text("Max Distance");
		ImGui::Spacing();
		ImGui::Text("Is Looped");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, m_SecondCollumnWidth);

		ImGui::SetNextItemWidth(m_ItemWidth);
		DragFloat("##MinDistance", audioSource, audioSource->GetMinDistance(),
			&AudioSource::SetMinDistance, 0.01f, 0.0f);

		ImGui::SetNextItemWidth(m_ItemWidth);
		DragFloat("##MaxDistance", audioSource, audioSource->GetMaxDistance(),
			&AudioSource::SetMaxDistance, 0.01f, 0.0f);

		Checkbox("##IsLooped", audioSource, audioSource->GetIsLooped(),
			&AudioSource::SetLooped);

		ImGui::Columns(1);
	}
}

void Inspector::RenderComponent(AudioListener* audioListener)
{
	bool isOpen = RenderComponentHeader("Audio Listener", audioListener, true);

	if (isOpen)
	{

	}
}

void Inspector::RenderAddComponentButton()
{
	int center = ImGui::GetWindowSize().x / 2;
	ImGui::SameLine(center - 100);
	if (ImGui::Button("Add Component", ImVec2(200, 20)))
	{
		ImGui::OpenPopup("Components");
	}

	if (ImGui::BeginPopup("Components"))
	{
		if (ImGui::MenuItem("Sprite Renderer"))
			m_SelectedGameObject->AddComponent<SpriteRenderer>();

		if (ImGui::MenuItem("Camera"))
			m_SelectedGameObject->AddComponent<Camera>();

		if (ImGui::MenuItem("Rigidbody"))
			m_SelectedGameObject->AddComponent<Rigidbody>();

		if (ImGui::MenuItem("Box Collider"))
			m_SelectedGameObject->AddComponent<BoxCollider>();

		if (ImGui::MenuItem("Circle Collider"))
			m_SelectedGameObject->AddComponent<CircleCollider>();

		if (ImGui::MenuItem("Polygon Collider"))
			m_SelectedGameObject->AddComponent<PolygonCollider>();

		if (ImGui::MenuItem("Animator"))
			m_SelectedGameObject->AddComponent<Animator>();

		if (ImGui::MenuItem("Audio Source"))
			m_SelectedGameObject->AddComponent<AudioSource>();

		if (ImGui::MenuItem("Audio Listener"))
			m_SelectedGameObject->AddComponent<AudioListener>();

		ImGui::Separator();

		for (auto& it : ScriptManager::Get().GetScripts())
		{
			if (ImGui::MenuItem(it.first.c_str()))
			{
				Script* script = it.second->Create();
				script->SetName(it.first);
				m_SelectedGameObject->AddScript(script);
			}
		}

		ImGui::EndPopup();
	}
}

void Inspector::RenderTagsAndLayersManager()
{
	if (ImGui::Button("<  "))
		m_TagsAndLayersManager = false;

#pragma region TagsManager
	if (ImGui::CollapsingHeader("Tags"))
	{
		auto RenderTag = [](std::string& tag)
		{
			std::string inputTag = tag;
			ImGui::InputText("##", &inputTag);

			if (ImGui::IsItemDeactivatedAfterEdit())
				tag = inputTag;

			ImGui::SameLine();
			if (ImGui::Button("-"))
				Tag::Remove(tag);
		};

		auto& tags = Tag::GetTags();

		if (ImGui::Button("+"))
		{
			Tag::Add("Tag"s + std::to_string(tags.size()));
		}

		for (size_t i = 1; i < tags.size(); i++)
		{
			ImGui::PushID((tags[i] + std::to_string(i)).c_str());
			RenderTag(tags[i]);
			ImGui::PopID();
		}
	}
#pragma endregion
}
