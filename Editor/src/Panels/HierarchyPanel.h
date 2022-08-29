#pragma once
#include "src/Core/Base.h"
#include "src/Scene/Scene.h"

class HierarchyPanel
{
private:
	Ptr<GameObject>& m_SelectedGameObject;

	char m_FindInput[20];
	std::string findStr;
private:
	void GameObjectPropertiesPopup();
	void RenderGameObjects();
	void RenderGameObjectTreeNode(GameObject* gameObject);
	void RenderSceneHeader();
	void AddGameObjectPopup();

public:
	HierarchyPanel(Ptr<GameObject>& gameObjectRef);

	void ImGuiRender();

	GameObject* GetSelectedGameObject();

	~HierarchyPanel()
	{
		std::cout << "Hierarchy destructor!!!\n";
	}
};