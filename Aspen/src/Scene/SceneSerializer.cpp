#include "SceneSerializer.h"
#include "../Log/Log.h"


SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{ }

#pragma region Serialization

void SceneSerializer::Serialize() const
{
	std::ofstream fileStream(m_Scene->m_Name + ".scene", std::ofstream::binary);

	if (!fileStream.is_open())
	{
		return;
		ERROR("Failed to open the file!!!");
	}

	json serializer =
	{
		{ "Scene", {
			{ "Name", m_Scene->m_Name},
			{ "Objects amount",  m_Scene->m_GameObjects.size() },
			{ "Gravity",{{"X", m_Scene->m_Gravity.x}, {"Y", m_Scene->m_Gravity.y}}}
		}}
	};

	for (size_t i = 0; i < m_Scene->m_GameObjects.size(); i++)
	{
		//SerializeGameObject(serializer[std::to_string(i)], m_Scene->m_GameObjects[i]);
	}

	fileStream << serializer;

	fileStream.close();
}

#pragma endregion

#pragma region Deserialization

void SceneSerializer::Deserialize()
{
	using namespace nlohmann;
	std::ifstream fileStream(m_Scene->m_Name + ".scene", std::ofstream::binary);

	if (!fileStream.is_open())
	{
		ERROR("Failed to open the file!!!");
		return;
	}

	json in;
	fileStream >> in;

	//DeserializeScene(in);

	fileStream.close();
}

void SceneSerializer::Deserialize(const std::string& filename)
{
	using namespace nlohmann;
	std::ifstream fileStream(filename, std::ofstream::binary);

	if (!fileStream.is_open())
	{
		ERROR("Failed to open the file!!!");
		return;
	}

	json in;
	fileStream >> in;

	//DeserializeScene(in);

	fileStream.close();
}

#pragma endregion