#pragma once
#include "../Panels/HierarchyPanel.h"
#include "../Panels/Inspector.h"
#include "../Panels/ProjectPanel.h"
#include "src/Renderer/Framebuffer.h"
#include "src/Scene/Scene.h"
#include "src/Scene/EditorCamera.h"
#include "imgui/imgui.h"

#include "Layer.h"

class Editor : public Layer
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

	Texture m_MoveButtonIcon;
	Texture m_RotateButtonIcon;
	Texture m_ScaleButtonIcon;

	GameObject* m_SelectedObject = nullptr;
	GameObject* m_HoveredObject = nullptr;

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
	
	void OpenScene();
	void SaveScene();
	void SaveSceneAs(); 

	void UpdateGuizmo();

public:
	Editor();

	void Update();
	void ImGuiRender();

	~Editor()
	{
		std::cout << "Editor destructor!!!\n";
	}
};