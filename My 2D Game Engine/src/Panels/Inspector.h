#pragma once

#include "HierarchyPanel.h"
#include "../GameObject/GameObject.h"

class Inspector
{
private:
	Ptr<GameObject>& m_SelectedGameObject;

public:
	Inspector(Ptr<GameObject>& gameObjectRef);
	void ImGuiRender();

	void DrawComponent(std::shared_ptr<Transform>& component);
	void DrawComponent(SpriteRenderer& component);
	//void DrawComponent(std::shared_ptr<Rigidbody>& component);
};
