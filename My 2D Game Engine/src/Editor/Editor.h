#pragma once
#include "../src/Panels/HierarchyPanel.h"
#include "../src/Panels/Inspector.h"
#include "../src/Panels/ProjectPanel.h"
#include "../src/Renderer/Framebuffer.h"

class Editor
{
private:
	enum class SceneState { PLAY = 0, EDIT = 1 };

	HierarchyPanel m_HierarchyPanel;
	Inspector      m_InspectorPanel;
	ProjectPanel   m_ProjectPanel;

	EditorCamera m_EditorCamera;

	SceneState m_SceneState = SceneState::EDIT;
	std::shared_ptr<Scene> m_ActiveScene;
	bool m_Pause = false;

	GameObject* m_SelectedObject = nullptr;
	GameObject* m_HoveredObject = nullptr;

	Framebuffer m_SceneFramebuffer;
	Framebuffer m_GameFramebuffer;

	ImVec2 m_SceneWindowSize;
public:
	Editor();

//	void SetSelectedObject(std::shared_ptr<GameObject> gameObject);
//	std::shared_ptr<GameObject> GetSelectedObject() { return m_SelectedObject; }

	void Update();

	void MenuBar();
	void DockSpaceBegin();
	void DockSpaceEnd();
	void SceneWindow();  // maybe move it to seperate class
	void GameWindow();   // maybe move it to seperate class
	void ImGuiRender();
};