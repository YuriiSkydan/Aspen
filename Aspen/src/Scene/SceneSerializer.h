#pragma once
#include "Scene.h"
#include "JSON/json.hpp"
using namespace nlohmann;

class ASPEN SceneSerializer
{
private:
	std::shared_ptr<Scene> m_Scene;

public:
	SceneSerializer(std::shared_ptr<Scene> scene);

	void Serialize() const;
	void Deserialize();
	void Deserialize(const std::string& filename);
};

