#include "SceneSerializer.h"
#include "../src/GameObject/GameObject.h"
#include "../Log/Log.h"


SceneSerializer::SceneSerializer(Scene* scene)
	:m_Scene(scene)
{
}

#pragma region Serialization

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
		SerializeComponent(out, transform);
	}

	if (gameObject->HasComponent<SpriteRenderer>())
	{
		SpriteRenderer* renderer = gameObject->GetComponent<SpriteRenderer>();
		SerializeComponent(out, renderer);
	}

	if (gameObject->HasComponent<BoxCollider>())
	{
		BoxCollider* boxCollider = gameObject->GetComponent<BoxCollider>();
		SerializeComponent(out, boxCollider);
	}

	if (gameObject->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		SerializeComponent(out, rigidbody);
	}
}

void SceneSerializer::SerializeComponent(json& out, Transform* transform)
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

void SceneSerializer::SerializeComponent(json& out, SpriteRenderer* spriteRenderer)
{
	const Color& color = spriteRenderer->GetColor();
	out["SpriteRenderer"] =
	{
		{ "Color",
			{{ "R", color.r },
			 { "G", color.g },
			 { "B", color.b },
			 { "A", color.a }}},
		{ "Texture", spriteRenderer->GetTexture().GetPath() }
	};
}

void SceneSerializer::SerializeComponent(json& out, BoxCollider* boxCollider)
{
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

void SceneSerializer::SerializeComponent(json& out, CircleCollider* circleCollider)
{
}

void SceneSerializer::SerializeComponent(json& out, Rigidbody* rigidbody)
{
	out["Rigidbody"] =
	{
		{ "Mass", rigidbody->GetMass() }
	};
}

void SceneSerializer::SerializeComponent(json& out, Camera* camera)
{
}

#pragma endregion

#pragma region Deserialization

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

void SceneSerializer::DeserializeGameObject(json& in, std::unique_ptr<GameObject>& gameObject)
{
	strcpy_s(gameObject->m_Name, 20, std::string(in["Name"]).c_str());
	gameObject->m_ID = in["ID"];
	gameObject->m_IsActive = in["IsActive"];

	DeserializeComponent(in["Transform"], gameObject->transform);

	if (in.find("SpriteRenderer") != in.end())
	{
		SpriteRenderer* spriteRenderer = gameObject->AddComponent<SpriteRenderer>();
		DeserializeComponent(in["SpriteRenderer"], spriteRenderer);
	}

	if (in.find("BoxCollider") != in.end())
	{
		BoxCollider* collider = gameObject->AddComponent<BoxCollider>();
		DeserializeComponent(in["BoxCollider"], collider);
	}

	if (in.find("CircleCollider") != in.end())
	{
		CircleCollider* collider = gameObject->AddComponent<CircleCollider>();
		DeserializeComponent(in["CircleCollider"], collider);
	}

	if (in.find("Rigidbody") != in.end())
	{
		Rigidbody* rigidbody = gameObject->AddComponent<Rigidbody>();
		DeserializeComponent(in["Rigidbody"], rigidbody);
	}

	if (in.find("Camera") != in.end())
	{
		Camera* camera = gameObject->AddComponent<Camera>();
		DeserializeComponent(in["Rigidbody"], camera);
	}
}

void SceneSerializer::DeserializeComponent(json& in, Transform* transform)
{
	transform->position.x = in["Position"]["X"];
	transform->position.y = in["Position"]["Y"];

	transform->angle = in["Angle"];

	transform->scale.x = in["Scale"]["X"];
	transform->scale.y = in["Scale"]["Y"];
}

void SceneSerializer::DeserializeComponent(json& in, SpriteRenderer* spriteRenderer)
{
	Color color;
	color.r = in["Color"]["R"];
	color.g = in["Color"]["G"];
	color.b = in["Color"]["B"];
	color.a = in["Color"]["A"];

	spriteRenderer->SetColor(color);
	spriteRenderer->SetSprite(in["Texture"]);
}

void SceneSerializer::DeserializeComponent(json& in, BoxCollider* collider)
{
	collider->size.x = in["Size"]["X"];
	collider->size.y = in["Size"]["Y"];

	collider->offset.x = in["Offset"]["X"];
	collider->offset.y = in["Offset"]["Y"];
}

void SceneSerializer::DeserializeComponent(json& in, CircleCollider* collider)
{
	collider->radius = in["Radius"];
}

void SceneSerializer::DeserializeComponent(json& in, Rigidbody* rigidbody)
{

}

void SceneSerializer::DeserializeComponent(json& in, Camera* camera)
{
}

#pragma endregion