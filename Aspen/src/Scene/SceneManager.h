#pragma once
#include "Scene.h"

class SceneManager
{
private:
	static std::shared_ptr<Scene> m_ActiveScene;

	friend class ProjectPanel;
	friend class Editor;
private:
	static void LoadScene(std::string_view filename);
	static void SetActiveScene(std::shared_ptr<Scene> activeScene);

public:
	static std::shared_ptr<Scene> GetActiveScene() { return m_ActiveScene; }
};