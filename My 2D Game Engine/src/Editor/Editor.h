#pragma once
#include "../src/Panels/HierarchyPanel.h"
#include "../src/Panels/Inspector.h"
#include "../src/Panels/ProjectPanel.h"
#include "../src/Renderer/Framebuffer.h"

class Editor
{
private:
	HierarchyPanel m_HierarchyPanel;
	Inspector      m_InspectorPanel;
	ProjectPanel   m_ProjectPanel;

	EditorCamera m_EditorCamera;

	enum class SceneState { PLAY = 0, EDIT = 1 };
	SceneState m_SceneState = SceneState::EDIT;
	
	std::shared_ptr<Scene> m_EditorScene;
	std::shared_ptr<Scene> m_ActiveScene;

	bool m_Pause = false;

	Texture m_PlayButtonIcon;
	Texture m_PauseButtonIcon;
	Texture m_StopButtonIcon;

	GameObject* m_SelectedObject = nullptr;
	GameObject* m_HoveredObject = nullptr;

	Framebuffer m_SceneFramebuffer;
	Framebuffer m_GameFramebuffer;

	ImVec2 m_SceneWindowSize;
	float m_ToolbarHeight;
	float m_MenuBarHeight;

private:
	void InitImGui();
	void ImGuiBegin();
	void ImGuiEnd();

	void BlackStyle();
	void CorporateGreyStyle();
	void DarkStyle();
	void DarkNightStyle();

	void Toolbar();
	void DockSpace();
	void MainMenuBar();
	void SceneWindow();  // maybe move it to seperate class
	void GameWindow();   // maybe move it to seperate class	
	
	void OpenScene();
	void SaveScene();
	void SaveSceneAs(); 

public:
	Editor();

	void Update();
	void ImGuiRender();
};