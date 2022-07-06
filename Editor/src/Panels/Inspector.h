#pragma once
#include "HierarchyPanel.h"
#include "src/GameObject/GameObject.h"
#include "imgui/imgui.h"

class Inspector
{
private:
	Ptr<GameObject>& m_SelectedGameObject;

	unsigned int m_FirstCollumnWidth = 110;
	unsigned int m_SecondCollumnWidth;
	unsigned int m_ItemWidth;

public:
	Inspector(Ptr<GameObject>& gameObjectRef);
	void ImGuiRender();

	template<typename... Components>
	void DrawComponents(GameObject* gameObject)
	{
		([&]()
			{
				Components* component = gameObject->GetComponent<Components>();
				if (component != nullptr)
				{
					DrawComponent(component);
					ImGui::Separator();
				}
			}(), ...);
	}

	void DrawComponent(Transform* transform);
	void DrawComponent(SpriteRenderer* spriteRenderer);
	void DrawComponent(Camera* camera);
	void DrawComponent(Rigidbody* rigidbody);
	void DrawComponent(BoxCollider* boxCollider);
	void DrawComponent(CircleCollider* circleCollider);
};
