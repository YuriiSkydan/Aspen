#pragma once
#include "src/Renderer/Framebuffer.h"
#include "src/Scene/Scene.h"
#include "src/Scene/EditorCamera.h"
#include "../Panels/HierarchyPanel.h"
#include "../Panels/Inspector.h"
#include "../Panels/ProjectPanel.h"

#include "../ImGuizmo/ImGuizmo.h"

#include "Layer.h"

class Editor : public Layer
{
private:
	HierarchyPanel m_HierarchyPanel;
	Inspector      m_InspectorPanel;
	ProjectPanel   m_ProjectPanel;

	EditorCamera m_EditorCamera;

	enum class SceneState { PLAY = 0, EDIT = 1, PAUSE = 2};
	SceneState m_SceneState = SceneState::EDIT;
	
	std::shared_ptr<Scene> m_EditorScene;
	//std::shared_ptr<Scene> m_ActiveScene;

	Texture m_PlayButtonIcon;
	Texture m_PauseButtonIcon;
	Texture m_StopButtonIcon;

	Texture m_MoveButtonIcon;
	Texture m_RotateButtonIcon;
	Texture m_ScaleButtonIcon;

	ImGuizmo::OPERATION m_CurrentOperation;

	GameObject* m_SelectedObject = nullptr;

	Framebuffer m_SceneFramebuffer;
	Framebuffer m_GameFramebuffer;

	ImVec2 m_SceneWindowSize;
	ImVec2 m_GameWindowSize;

	float m_ToolbarHeight;
	float m_MenuBarHeight;

private:
	void BlackStyle();
	void CorporateGreyStyle();
	void DarkStyle();
	void DarkNightStyle();

	void Toolbar(); // move it to seperate class later
	void DockSpace();
	void MainMenuBar();
	void SceneWindow();  // maybe move it to seperate class
	void GameWindow();   // maybe move it to seperate class	
	void ProjectPreferencesWindow();

	void OpenScene();
	void OpenScene(std::string_view path);
	void SaveScene();
	void SaveSceneAs(); 

	void UpdateGuizmo();
	void PickHoveredObject();
public:
	Editor();

	void Update();
	void ImGuiRender();
};