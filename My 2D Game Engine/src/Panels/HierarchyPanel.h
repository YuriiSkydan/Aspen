#pragma once
#include "../Engine/Base.h"
#include "../Scene/Scene.h"

class HierarchyPanel
{
private:
	std::shared_ptr<Scene>& m_Scene;
	Ptr<GameObject>& m_SelectedGameObject;

	char findInput[20];
	std::string findStr;
public:
	//HierarchyPanel(Ptr<GameObject>& gameObjectRef);
	HierarchyPanel(std::shared_ptr<Scene>& scene, Ptr<GameObject>& gameObjectRef);

	//static void Init(const std::shared_ptr<Scene>& scene);
	//void SetScene(const std::shared_ptr<Scene>& scene);
	//	void SetSelectedGameObjectRef(std::shared_ptr<GameObject>& gameObject);
	void ImGuiRender();

	GameObject* GetSelectedGameObject();
};