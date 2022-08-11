#pragma once
#include "src/Core/Base.h"
#include "src/Scene/Scene.h"

class HierarchyPanel
{
private:
	std::shared_ptr<Scene>& m_Scene;
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
	HierarchyPanel(std::shared_ptr<Scene>& scene, Ptr<GameObject>& gameObjectRef);

	void ImGuiRender();

	GameObject* GetSelectedGameObject();

	~HierarchyPanel()
	{
		std::cout << "Hierarchy destructor!!!\n";
	}
};