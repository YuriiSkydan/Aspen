#pragma once
#include "HierarchyPanel.h"
#include "src/GameObject/GameObject.h"
#include "imgui/imgui.h"

class Inspector
{
private:
	Ptr<GameObject>& m_SelectedGameObject;

	float m_MinWindowWidth = 200;
	ImVec2 m_PanelSize;

public:
	Inspector(Ptr<GameObject>& gameObjectRef);
	void ImGuiRender();

	void DrawComponent(std::shared_ptr<Transform>& component);
	void DrawComponent(SpriteRenderer& component);
	//void DrawComponent(std::shared_ptr<Rigidbody>& component);
};
