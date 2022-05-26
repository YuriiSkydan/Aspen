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
	std::unique_ptr<Scene> m_ActiveScene;
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

	enum class Theme { Dark, Black, DarkNight, CorporateGray, Light };
	Theme m_CurrentTheme = Theme::Dark;

private:
	void BlackStyle();
	void CorporateGreyStyle();
	void DarkStyle();
	void DarkNightStyle();
	void SetCurrentTheme();

	void Toolbar();
	void DockSpace();
	void MainMenuBar();
	void SceneWindow();  // maybe move it to seperate class
	void GameWindow();   // maybe move it to seperate class	
	
	void OpenScene();
	void SaveScene();

public:
	Editor();

	void Update();
	void ImGuiRender();
};