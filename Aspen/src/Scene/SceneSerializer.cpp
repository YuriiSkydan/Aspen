#include "SceneSerializer.h"
#include "../Log/Log.h"


SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

#pragma region Serialization

void SceneSerializer::Serialize() const
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

void SceneSerializer::SerializeGameObject(json& out, const std::unique_ptr<GameObject>& gameObject) const
{
	out =
	{
		{ "Name",  gameObject->m_Name },
		{ "ID",  gameObject->m_ID },
		{ "IsActive",  gameObject->m_IsActive }
	};
	
	SerializeComponents<AllComponents>(out, gameObject);
}

void SceneSerializer::SerializeComponent(json& out, Transform* transform) const
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

void SceneSerializer::SerializeComponent(json& out, SpriteRenderer* spriteRenderer) const
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

void SceneSerializer::SerializeComponent(json& out, BoxCollider* boxCollider) const
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

void SceneSerializer::SerializeComponent(json& out, CircleCollider* circleCollider) const
{
	out["CircleCollider"] =
	{
		{ "Radius", circleCollider->radius }
	};
}

void SceneSerializer::SerializeComponent(json& out, Rigidbody* rigidbody) const
{
	out["Rigidbody"] =
	{
		{ "Mass", rigidbody->GetMass() },
		{ "LinearDrag", rigidbody->GetLinearDrag() },
		{ "AngularDrag", rigidbody->GetAngularDrag() },
		{ "BodyType", int(rigidbody->type) },
		{ "GravityScale", rigidbody->gravityScale },
		{ "FixedRotation", rigidbody->fixedRotation }
	};
}

void SceneSerializer::SerializeComponent(json& out, Camera* camera) const
{
	out["Camera"] =
	{
		{ "Size", camera->size},
		{ "Aspect ration", camera->GetAspectRation()},
		{ "Background color",
		{ { "R", camera->backgroundColor.r},
		  { "G", camera->backgroundColor.g},
		  { "B", camera->backgroundColor.b},
		  { "A", camera->backgroundColor.a} }}
	};
}

void SceneSerializer::SerializeComponent(json& out, PolygonCollider* collider) const
{
}

void SceneSerializer::SerializeComponent(json& out, Animator* animator) const
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

	json in;
	fileStream >> in;

	m_Scene->m_Name = in["Scene"]["Name"];
	m_Scene->m_GameObjects.resize(in["Scene"]["Objects amount"]);
	m_Scene->m_Gravity.x = in["Scene"]["Gravity"]["X"];
	m_Scene->m_Gravity.y = in["Scene"]["Gravity"]["Y"];

	for (size_t i = 0; i < m_Scene->m_GameObjects.size(); i++)
	{
		auto newGameObject = std::make_unique<GameObject>(m_Scene.get());
		DeserializeGameObject(in[std::to_string(i)], newGameObject);
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
		DeserializeComponent(in["Camera"], camera);
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
	rigidbody->SetMass(in["Mass"]);
	rigidbody->SetLinearDrag(in["LinearDrag"]);
	rigidbody->SetAngularDrag(in["AngularDrag"]);
	rigidbody->type = BodyType(in["BodyType"]);
	rigidbody->gravityScale = in["GravityScale"];
	rigidbody->fixedRotation = in["FixedRotation"];
}

void SceneSerializer::DeserializeComponent(json& in, Camera* camera)
{
	camera->size = in["Size"];
	camera->SetRatio(in["Aspect ration"]);

	Color color;
	color.r = in["Background color"]["R"];
	color.g = in["Background color"]["G"];
	color.b = in["Background color"]["B"];
	color.a = in["Background color"]["A"];
	camera->backgroundColor = color;
}

void SceneSerializer::DeserializeComponent(json& in, PolygonCollider* collider)
{
}

void SceneSerializer::DeserializeComponent(json& in, Animator* animator)
{
}

#pragma endregion