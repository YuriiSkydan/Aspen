#include "HierarchyPanel.h"
#include "src/Scene/SceneManager.h"
#include "Components/Transform.h"
#include "imgui/imgui.h"

using namespace std::string_literals;

HierarchyPanel::HierarchyPanel(Ptr<GameObject>& gameObjectRef)
	: m_SelectedGameObject(gameObjectRef)
{ }

void HierarchyPanel::GameObjectPropertiesPopup()
{
	if (ImGui::MenuItem("Delete"))
	{
		SceneManager::GetActiveScene()->DestroyGameObject(m_SelectedGameObject);
		m_SelectedGameObject = nullptr;
	}
}

void HierarchyPanel::RenderGameObjectTreeNode(GameObject* gameObject)
{
	ImGui::PushID(("##"s + gameObject->GetName() + std::to_string(gameObject->GetID())).c_str());

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (gameObject == m_SelectedGameObject)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (gameObject->transform->GetChilds().size() == 0)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	bool nodeOpen = ImGui::TreeNodeEx(gameObject->GetName(), nodeFlags);

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("HIERARCHY_PANEL_ITEM", gameObject, sizeof(GameObject));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_PANEL_ITEM"))
		{
			GameObject* dragObject = (GameObject*)payload->Data;
			if (dragObject != gameObject)
			{
				if (dragObject->transform->GetParent() == nullptr ||
					dragObject->transform->GetParent()->gameObject != gameObject)
				{
					//if (dragObject->transform->parent != nullptr)
					//	dragObject->transform->parent->child = nullptr;

					dragObject->transform->SetParent(gameObject->transform);
					ImGui::EndDragDropTarget();
				}
			}
		}
	}

	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_SelectedGameObject = gameObject;

		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{

			m_SelectedGameObject = gameObject;
			ImGui::OpenPopup("Object Properties", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopup);
		}
	}

	if (ImGui::BeginPopup("Object Properties"))
	{
		GameObjectPropertiesPopup();
		ImGui::EndPopup();
	}

	ImGui::PopID();

	if (nodeOpen && !(nodeFlags & ImGuiTreeNodeFlags_Leaf))
	{
		auto& childs = gameObject->transform->GetChilds();
		for (auto& child : childs)
		{
			RenderGameObjectTreeNode(child->gameObject);
		}
		ImGui::TreePop();
	}
}

void HierarchyPanel::RenderSceneHeader()
{
	auto& objects = SceneManager::GetActiveScene()->m_GameObjects;

	if (strlen(m_FindInput) != 0)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			std::string objName = objects[i]->GetName();
			if (objName.find(m_FindInput) != std::string::npos)
				RenderGameObjectTreeNode(objects[i].get());
		}
	}
	else
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->transform->GetParent() == nullptr)
				RenderGameObjectTreeNode(objects[i].get());
		}
	}

	ImGui::Dummy(ImGui::GetWindowSize());

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_PANEL_ITEM"))
		{
			GameObject* dragObject = (GameObject*)payload->Data;
			dragObject->transform->SetParent(nullptr);
			ImGui::EndDragDropTarget();
		}
	}
}

void HierarchyPanel::AddGameObjectPopup()
{
	if (ImGui::MenuItem("Create Empty"))
	{
		SceneManager::GetActiveScene()->CreateGameObject();
	}
	if (ImGui::MenuItem("Create Sprite"))
	{
		GameObject* gameObject = SceneManager::GetActiveScene()->CreateGameObject();
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->SetName("Sprite");
	}
	if (ImGui::MenuItem("Create Camera"))
	{
		GameObject* gameObject = SceneManager::GetActiveScene()->CreateGameObject();
		gameObject->AddComponent<Camera>();
		gameObject->AddComponent<AudioListener>();
		gameObject->SetName("Camera");
	}
	if (ImGui::MenuItem("Rigidbody Square"))
	{
		GameObject* gameObject = SceneManager::GetActiveScene()->CreateGameObject();
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Rigidbody>();
		gameObject->AddComponent<BoxCollider>();
	}
}

void HierarchyPanel::ImGuiRender()
{
	ImGui::Begin("Hierarchy");

	if (ImGui::Button("+"))
		ImGui::OpenPopup("Game Objects");

	if (ImGui::BeginPopup("Game Objects"))
	{
		AddGameObjectPopup();
		ImGui::EndPopup();
	}

	ImGui::SameLine();
	ImGui::InputText("##", m_FindInput, 20);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader(SceneManager::GetActiveScene()->GetName().c_str(), flags))
		RenderSceneHeader();
	
	ImGui::End();
}

GameObject* HierarchyPanel::GetSelectedGameObject()
{
	return m_SelectedGameObject;
}