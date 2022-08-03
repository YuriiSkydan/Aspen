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

void HierarchyPanel::GameObjectPropertiesPopup()
{
	if (ImGui::MenuItem("Delete"))
	{
		//change it later
		//gameObjects.erase(gameObjects.begin() + i);

		m_Scene->DestroyGameObject(m_SelectedGameObject);
		m_SelectedGameObject = nullptr;
	}
}

void HierarchyPanel::RenderGameObjectTreeNode(GameObject* gameObject)
{
	ImGui::PushID(("##"s + gameObject->GetName()).c_str());

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

HierarchyPanel::HierarchyPanel(std::shared_ptr<Scene>& scene, Ptr<GameObject>& gameObjectRef)
	:m_Scene(scene), m_SelectedGameObject(gameObjectRef)
{
}

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
		for (auto& object : m_Scene->m_GameObjects)
		{
			if (object->transform->GetParent() == nullptr)
				RenderGameObjectTreeNode(object.get());
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

	ImGui::End();
}

GameObject* HierarchyPanel::GetSelectedGameObject()
{
	return m_SelectedGameObject;
}
