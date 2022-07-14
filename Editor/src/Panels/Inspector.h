#pragma once
#include "HierarchyPanel.h"
#include "src/GameObject/GameObject.h"

#ifndef IMGUI_API
#undef IMGUI_API
#endif

#define IMGUI_API __declspec(dllimport)

#include "imgui/imgui.h"

class Inspector
{
private:
	Ptr<GameObject>& m_SelectedGameObject;

	unsigned int m_FirstCollumnWidth = 110;
	unsigned int m_SecondCollumnWidth;
	unsigned int m_ItemWidth;

private:
	void DrawGameObjectProperties();
	void DrawComponents();

	template<typename... Components>
	void DrawComponents(Component* component)
	{
		bool hasDrown = false;

		([&]()
			{
				if (!hasDrown)
				{
					Components* castComponent = dynamic_cast<Components*>(component);
					if (castComponent != nullptr)
					{
						DrawComponent(castComponent);
						ImGui::Separator();
						hasDrown = true;
					}
				}
			}(), ...);
	}

	bool DrawComponentHeader(const std::string& componentName, Component* component, bool isEditable);
	void DrawComponent(Transform* transform);
	void DrawComponent(SpriteRenderer* spriteRenderer);
	void DrawComponent(Camera* camera);
	void DrawComponent(Rigidbody* rigidbody);
	void DrawComponent(BoxCollider* boxCollider);
	void DrawComponent(CircleCollider* circleCollider);
	void DrawComponent(PolygonCollider* polygonCollider);

	void ImGuiAddComponentButton();

public:
	Inspector(Ptr<GameObject>& gameObjectRef);
	void ImGuiRender();
};
