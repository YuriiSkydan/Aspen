#pragma once
#include "Scene.h"

class ASPEN SceneManager
{
private:
	inline static SceneManager* s_Instance = nullptr;
	std::shared_ptr<Scene> m_ActiveScene;

	friend class ProjectPanel;
	friend class Editor;
private:
	static void LoadScene(std::string_view filename);
	static void SetActiveScene(std::shared_ptr<Scene> activeScene);

public:
	SceneManager();
	static std::shared_ptr<Scene> GetActiveScene() { return s_Instance->m_ActiveScene; }
};