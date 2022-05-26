#include "SceneSerializer.h"
#include "../src/GameObject/GameObject.h"
#include "../Log/Log.h"

void SceneSerializer::SerializeGameObject(json& out, std::unique_ptr<GameObject>& gameObject)
{
	out =
	{
		{ "Name",  gameObject->m_Name },
		{ "ID",  gameObject->m_ID },
		{ "IsActive",  gameObject->m_IsActive }
	};

	Transform* transform = gameObject->transform;
	if (transform != nullptr)
	{
		out["Transform"] =
		{
			{ "Position",
				{{ "X", transform->position.x },
				 { "Y", transform->position.y }}},
			{ "Angle", transform->angle },
			{ "Scale",
				{{"X", transform->scale.x},
				 {"Y", transform->scale.y }}}
		};
	}

	if (gameObject->HasComponent<SpriteRenderer>())
	{
		SpriteRenderer* renderer = gameObject->GetComponent<SpriteRenderer>();
		const Color& color = renderer->GetColor();
		out["SpriteRenderer"] =
		{
			{ "Color",
				{{ "R", color.r },
				 { "G", color.g },
				 { "B", color.b },
				 { "A", color.a }}},
			{ "Texture", renderer->GetTexture().GetPath() }
		};
	}

	if (gameObject->HasComponent<BoxCollider>())
	{
		BoxCollider* boxCollider = gameObject->GetComponent<BoxCollider>();
		out["BoxCollider"] =
		{
			{ "Size",
			{ { "X", boxCollider->size.x },
			  { "Y", boxCollider->size.y } }},
			{ "Offset",
			{ { "X", boxCollider->offset.x },
			  { "Y", boxCollider->offset.y } }}
		};
	}

	if (gameObject->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		out["Rigidbody"] =
		{
			{ "Mass", rigidbody->GetMass() }
		};
	}
}

void SceneSerializer::DeserializeGameObject(json& in, std::unique_ptr<GameObject>& gameObject)
{
	strcpy_s(gameObject->m_Name, 20, std::string(in["Name"]).c_str());
	gameObject->m_ID = in["ID"];
	gameObject->m_IsActive = in["IsActive"];

	Transform* transform = gameObject->transform;
	transform->position.x = in["Transform"]["Position"]["X"];
	transform->position.y = in["Transform"]["Position"]["Y"];

	transform->angle = in["Transform"]["Angle"];

	transform->scale.x = in["Transform"]["Scale"]["X"];
	transform->scale.y = in["Transform"]["Scale"]["Y"];

	if (in.find("SpriteRenderer") != in.end())
	{
		SpriteRenderer* spriteRenderer = gameObject->AddComponent<SpriteRenderer>();
		Color color;
		color.r = in["SpriteRenderer"]["Color"]["R"];
		color.g = in["SpriteRenderer"]["Color"]["G"];
		color.b = in["SpriteRenderer"]["Color"]["B"];
		color.a = in["SpriteRenderer"]["Color"]["A"];

		spriteRenderer->SetColor(color);
		spriteRenderer->SetSprite(in["SpriteRenderer"]["Texture"]);
	}

	if (in.find("BoxCollider") != in.end())
	{
		BoxCollider* boxCollider = gameObject->AddComponent<BoxCollider>();
		boxCollider->size.x = in["BoxCollider"]["Size"]["X"];
		boxCollider->size.y = in["BoxCollider"]["Size"]["Y"];

		boxCollider->offset.x = in["BoxCollider"]["Offset"]["X"];
		boxCollider->offset.y = in["BoxCollider"]["Offset"]["Y"];
	}

	if (in.find("Rigidbody") != in.end())
	{
		Rigidbody* rigidbody = gameObject->AddComponent<Rigidbody>();
		//rigidbody->SetMass(in["Rigidbody"]["Mass"]);
	}
}

SceneSerializer::SceneSerializer(Scene* scene)
	:m_Scene(scene)
{
}

void SceneSerializer::Serialize()
{
	std::ofstream fileStream(m_Scene->m_Name + ".json", std::ofstream::binary);

	if (!fileStream.is_open())
	{
		ERROR("Failed to open the file!!!");
		return;
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
		SerializeGameObject(serializer[std::to_string(i)], m_Scene->m_GameObjects[i]);
	}

	fileStream << serializer;

	fileStream.close();
}

void SceneSerializer::Deserialize()
{
	using namespace nlohmann;
	std::ifstream fileStream(m_Scene->m_Name + ".json", std::ofstream::binary);

	if (!fileStream.is_open())
	{
		ERROR("Failed to open the file!!!");
		return;
	}

	json deserializer;
	fileStream >> deserializer;

	m_Scene->m_Name = deserializer["Scene"]["Name"];
	m_Scene->m_GameObjects.resize(deserializer["Scene"]["Objects amount"]);
	m_Scene->m_Gravity.x = deserializer["Scene"]["Gravity"]["X"];
	m_Scene->m_Gravity.y = deserializer["Scene"]["Gravity"]["Y"];

	for (size_t i = 0; i < m_Scene->m_GameObjects.size(); i++)
	{
		auto newGameObject = std::make_unique<GameObject>(m_Scene);
		DeserializeGameObject(deserializer[std::to_string(i)], newGameObject);
		m_Scene->m_GameObjects[i] = std::move(newGameObject);
	};

	fileStream.close();
}