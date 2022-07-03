#include "Inspector.h"
#include "src/Log/Log.h"
#include "src/ScriptManager.h"
#include "src/Components/Script.h"
#include "src/Components/BoxCollider.h"
#include "src/Components/CircleCollider.h"
#include "src/Components/Rigidbody.h"
#include "imgui/imgui.h"

Inspector::Inspector(Ptr<GameObject>& gameObjectRef)
	:m_SelectedGameObject(gameObjectRef)
{}

void Inspector::ImGuiRender()
{
	ImGui::Begin("Inspector");

	if (m_SelectedGameObject != nullptr)
	{
		ImGui::Checkbox("##IsActive", &m_SelectedGameObject->m_IsActive);
		ImGui::SameLine();
		ImGui::InputText("##", m_SelectedGameObject->m_Name, 20);

		if (ImGui::Button("Tags"))
			ImGui::OpenPopup("Tags");

		ImGui::SameLine();

		if (ImGui::Button("Layers"))
			ImGui::OpenPopup("Layers");

		ImGui::Separator();

		//m_SelectedGameObject->UpdateComponentsGUI();

		for (auto& component : m_SelectedGameObject->GetComponents())
		{
			/*std::string componentName = typeid(*component).name();
			if (componentName == typeid(SpriteRenderer).name())
			{
				SpriteRenderer& spriteRenderer = dynamic_cast<SpriteRenderer&>(*component);
				DrawComponent(spriteRenderer);
			}*/

			component->UpdateGui();
			ImGui::Separator();
		}

		for (auto& script : m_SelectedGameObject->GetScripts())
		{
			ImGui::CollapsingHeader(script->GetName().c_str());
			script->UpdateGui();
			ImGui::Separator();
		}
		

		ImGui::NewLine();
		ImGui::NewLine();
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

			if (ImGui::MenuItem("Rigidbody"))
				m_SelectedGameObject->AddComponent<Rigidbody>();

			if (ImGui::MenuItem("Box Collider"))
				m_SelectedGameObject->AddComponent<BoxCollider>();

			if (ImGui::MenuItem("Circle Collider"))
				m_SelectedGameObject->AddComponent<CircleCollider>();

			if (ImGui::MenuItem("Camera"))
				m_SelectedGameObject->AddComponent<Camera>();

			ImGui::Separator();

			for (auto& it : ScriptManager::GetInstance().GetScripts())
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

	m_PanelSize = ImGui::GetWindowSize();
	ImGui::End();
}

void Inspector::DrawComponent(std::shared_ptr<Transform>& component)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat2("Position", (float*)&component->position, 0.01f);
		ImGui::DragFloat("Rotation", &component->angle, 0.05f);
		ImGui::DragFloat2("Scale", (float*)&component->scale, 0.01f);
	}
}

void Inspector::DrawComponent(SpriteRenderer& component)
{
	if (ImGui::CollapsingHeader("Sprite Renderer"))
	{
		Color color;
		if (ImGui::ColorEdit4("Color", (float*)&color))
		{
			component.SetColor(color);
		}
	}
}

//void Inspector::DrawComponent(std::shared_ptr<Rigidbody>& component)
//{
//	if (m_SelectedGameObject != nullptr)
//	{
//		if (ImGui::CollapsingHeader("Rigidbody"))
//		{
//			float mass = component->GetMass();
//			if (ImGui::DragFloat("Mass", &mass, 0.01f, 0.0f))
//			{
//				component->SetMass(mass);
//			}
//
//			ImGui::DragFloat2("Velocity", (float*)&component->velocity);
//		}
//	}
//}