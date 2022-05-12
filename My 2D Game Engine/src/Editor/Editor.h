#pragma once

#ifndef EDITOR_H
#define EDITOR_H

#include "../src/Panels/HierarchyPanel.h"
#include "../src/Panels/Inspector.h"
#include "../src/Panels/ProjectPanel.h"
#include "../src/Renderer/Framebuffer.h"

class Editor
{
private:
	enum class SceneState { PLAY = 0, EDIT = 1 };

	HierarchyPanel m_HierarchyPanel;
	Inspector m_InspectorPanel;
	ProjectPanel m_ProjectPanel;

	EditorCamera m_EditorCamera;

	std::shared_ptr<Scene> m_ActiveScene;
	std::shared_ptr<GameObject> m_GameObjectsData;

	SceneState m_SceneState = SceneState::EDIT;
	bool m_Pause = false;

	GameObject* m_SelectedObject = nullptr;

	Framebuffer m_SceneFramebuffer;
	Framebuffer m_GameFramebuffer;

	ImVec2 m_SceneWindowSize;

	ImVec2 start, size;
public:
	Editor();

//	void SetSelectedObject(std::shared_ptr<GameObject> gameObject);
//	std::shared_ptr<GameObject> GetSelectedObject() { return m_SelectedObject; }

	void Update();

	void MenuBar();
	void DockSpaceBegin();
	void DockSpaceEnd();
	void SceneWindow(); // maybe move it to seperate class
	void GameWindow();  // maybe move it to seperate class
	void ImGuiRender();
};

#endif
