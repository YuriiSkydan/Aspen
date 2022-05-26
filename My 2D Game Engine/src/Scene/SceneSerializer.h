#pragma once
#include "Scene.h"
#include "JSON/json.hpp"
using namespace nlohmann;

class SceneSerializer
{
private:
	Scene* m_Scene;
	json test;
private:
	void SerializeGameObject(json& out, std::unique_ptr<GameObject>& gameObject);
	void DeserializeGameObject(json& in, std::unique_ptr<GameObject>& gameObject);

public:
	SceneSerializer(Scene* scene);

	void Serialize();
	void Deserialize();
};

