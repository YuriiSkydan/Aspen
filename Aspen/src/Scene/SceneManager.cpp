#include "SceneManager.h"

SceneManager::SceneManager()
{
	s_Instance = this;
}

void SceneManager::LoadScene(std::string_view filename)
{
	s_Instance->m_ActiveScene->Deserialize(filename);
}

void SceneManager::SetActiveScene(std::shared_ptr<Scene> activeScene)
{
	s_Instance->m_ActiveScene = activeScene;
}
