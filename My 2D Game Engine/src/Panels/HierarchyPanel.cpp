#include "HierarchyPanel.h"
#include "../GameObject/GameObject.h"
#include "../Log/Log.h"


HierarchyPanel::HierarchyPanel(Ptr<GameObject>& gameObjectRef)
	:m_SelectedGameObject(gameObjectRef)
{
}

HierarchyPanel::HierarchyPanel(Scene* scene, Ptr<GameObject>& gameObjectRef)
	:m_SelectedGameObject(gameObjectRef)
{

}

void HierarchyPanel::SetScene(Scene* scene)
{
	m_Scene = scene;
}

void HierarchyPanel::ImGuiRender()
{
	ImGui::Begin("Hierarchy");

	//Add here scene name
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

	ImGui::InputText("##", findInput, 20);


	if (ImGui::CollapsingHeader(m_Scene->GetName().c_str(), flags))
	{
		if (strlen(findInput) != 0)
		{
			for (auto& object : m_Scene->m_GameObjects)
			{
				findStr = object->GetName();
				if (!findStr.find(findInput) && ImGui::Button(object->GetName()))
					m_SelectedGameObject = object.get();
			}
		}
		else
		{
			auto& gameObjects = (m_Scene->m_GameObjects);
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				ImGui::Text("   ");
				ImGui::SameLine();
				if (ImGui::MenuItem((gameObjects[i])->GetName()))
				{
					m_SelectedGameObject = gameObjects[i].get();
				}
				else if (ImGui::IsItemHovered() &&
					ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("Object Properties");
				}

				if (ImGui::BeginPopup("Object Properties"))
				{
					if (ImGui::MenuItem("Delete"))
					{
						//change it later
						gameObjects.erase(gameObjects.begin() + i);

						//m_Scene->DestroyGameObject(object);
						m_SelectedGameObject = nullptr;
					}
					ImGui::EndPopup();
				}
			}
		}
	}

	ImGui::End();
}

GameObject* HierarchyPanel::GetSelectedGameObject()
{
	return m_SelectedGameObject;
}
