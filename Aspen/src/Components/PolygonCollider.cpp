#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "../GameObject/GameObject.h"
#include "../Math/Math.h"
#include "../Physics/Physics.h"
#include "PolygonCollider.h"
#include "Rigidbody.h"

PolygonCollider::PolygonCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{
	m_Vertices.push_back({ -0.5f, -0.5f });
	m_Vertices.push_back({ 0.5f, -0.5f });
	m_Vertices.push_back({ 0.0f,  0.5f });
}

void PolygonCollider::Start()
{
	b2Body* body = nullptr;
	float mass = 1.0f;
	if (gameObject->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		body = rigidbody->GetBody();
		mass = rigidbody->GetMass();
	}
	else
	{
		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		bodyDef.angle = ToRads(-transform->angle);
		body = Physics::CreateBody(bodyDef);
	}

	b2PolygonShape shape;
	shape.Set(m_Vertices.data(), m_Vertices.size());

	SetFixtureDef();
	m_FixtureDef.shape = &shape;

	body->CreateFixture(&m_FixtureDef);
}

void PolygonCollider::AddVertex(const Vector2f& vertexPos)
{
	m_Vertices.push_back({ vertexPos.x, vertexPos.y });
}

void PolygonCollider::Serialize(json& out) const
{
	Collider::Serialize(out);

	out["PolygonCollider"] =
	{
		{ "Vertices", m_Vertices.size() }
	};

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		json& vertexOut = out["PolygonCollider"]["Vertex"];
		vertexOut[std::to_string(i)]["X"] = m_Vertices[i].x;
		vertexOut[std::to_string(i)]["Y"] = m_Vertices[i].y;
	}
}

void PolygonCollider::Deserialize(json& in)
{
	Collider::Deserialize(in);

	m_Vertices.resize(in["Vertices"]);

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		m_Vertices[i].x = in["Vertex"][std::to_string(i)]["X"];
		m_Vertices[i].y = in["Vertex"][std::to_string(i)]["Y"];
	}
}