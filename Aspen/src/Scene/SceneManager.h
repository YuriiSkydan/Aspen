#pragma once
#include "Scene.h"


class SceneManager
{
private:
	std::shared_ptr<Scene> m_ActiveScene;

private:
	void LoadScene(std::string_view filename);

public:
	std::shared_ptr<Scene> GetActiveScene() const { return m_ActiveScene; }
	const std::shared_ptr<Scene> GetActiveSceneRef() const { return m_ActiveScene; }
	void SetActiveScene(std::shared_ptr<Scene> activeScene);
};