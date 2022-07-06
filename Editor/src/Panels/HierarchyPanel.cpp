#include "HierarchyPanel.h"
#include "src/GameObject/GameObject.h"
#include "src/Components/BoxCollider.h"
#include "src/Components/CircleCollider.h"
#include "src/Components/Rigidbody.h"
#include "src/Log/Log.h"
#include "imgui/imgui.h"

using namespace std::string_literals;

//HierarchyPanel::HierarchyPanel(Ptr<GameObject>& gameObjectRef)
//	:m_SelectedGameObject(gameObjectRef)
//{
//}

HierarchyPanel::HierarchyPanel(std::shared_ptr<Scene>& scene, Ptr<GameObject>& gameObjectRef)
	:m_Scene(scene), m_SelectedGameObject(gameObjectRef)
{}

//void HierarchyPanel::SetScene(const std::shared_ptr<Scene>& scene)
//{
//	m_Scene = scene;
//}

void HierarchyPanel::ImGuiRender()
{
	ImGui::Begin("Hierarchy");

	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::Button("+"))
		ImGui::OpenPopup("Game Objects");

	if (ImGui::BeginPopup("Game Objects"))
	{
		if (ImGui::MenuItem("Create Empty"))
		{
			m_Scene->CreateGameObject();
		}
		if (ImGui::MenuItem("Create Sprite"))
		{
			GameObject* gameObject = m_Scene->CreateGameObject();
			gameObject->AddComponent<SpriteRenderer>();
			gameObject->SetName("Sprite");
		}
		if (ImGui::MenuItem("Create Camera"))
		{
			GameObject* gameObject = m_Scene->CreateGameObject();
			gameObject->AddComponent<Camera>();
			gameObject->SetName("Camera");
		}
		if (ImGui::MenuItem("Rigidbody Square"))
		{
			GameObject* gameObject = m_Scene->CreateGameObject();
			gameObject->AddComponent<SpriteRenderer>();
			gameObject->AddComponent<Rigidbody>();
			gameObject->AddComponent<BoxCollider>();
		}

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	ImGui::InputText("##", m_FindInput, 20);

	if (ImGui::CollapsingHeader(m_Scene->GetName().c_str(), flags))
	{
		if (strlen(m_FindInput) != 0)
		{
			for (auto& object : m_Scene->m_GameObjects)
			{
				findStr = object->GetName();
				if (!findStr.find(m_FindInput))
				{
					ImGui::Text("   ");
					ImGui::SameLine();
					if (ImGui::MenuItem(object->GetName()))
						m_SelectedGameObject = object.get();
				}
			}
		}
		else
		{
			for (auto& object : m_Scene->m_GameObjects)
			{
				ImGui::Text("   ");
				ImGui::SameLine();

				ImGui::PushID(("##"s + object->GetName()).c_str());
				if (ImGui::MenuItem(object->GetName()))
				{
					m_SelectedGameObject = object.get();
				}
				else if (ImGui::IsItemHovered() &&
					ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					m_SelectedGameObject = object.get();
					ImGui::OpenPopup("Object Properties");
				}
				ImGui::PopID();
			}
		}
	}

	if (ImGui::BeginPopup("Object Properties"))
	{
		if (ImGui::MenuItem("Delete"))
		{
			//change it later
			//gameObjects.erase(gameObjects.begin() + i);

			//m_Scene->DestroyGameObject(object);
			m_SelectedGameObject = nullptr;
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

GameObject* HierarchyPanel::GetSelectedGameObject()
{
	return m_SelectedGameObject;
}
