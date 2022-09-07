#include "BoxCollider.h"
#include "Rigidbody.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Physics.h"
#include "../Math/Math.h"

void BoxCollider::SetShape()
{
	float sizeX = size.x * transform->scale.x;
	float sizeY = size.y * transform->scale.y;
	b2Vec2 center = b2Vec2(m_Offset.x, m_Offset.y);

	m_Shape.SetAsBox(abs(sizeX), abs(sizeY), center, 0);
}

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void BoxCollider::Start()
{
	float mass = 1.0f;
	if (gameObject->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		m_Body = rigidbody->GetBody();
		mass = rigidbody->GetMass();
	}
	else
	{
		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		bodyDef.angle = ToRads(-transform->angle);
		m_Body = Physics::CreateBody(bodyDef);
	}

	SetShape();
	SetFixtureDef();
	m_FixtureDef.shape = &m_Shape;
	m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
}

void BoxCollider::Reset()
{
	Collider::Reset();
	size = { 0.5f, 0.5f };
}

void BoxCollider::SetOffset(const Vector2f& offset)
{
	m_Offset = offset;
	SetShape();

	if (m_Body != nullptr)
	{
		m_Body->CreateFixture(&m_FixtureDef);
	}
}

void BoxCollider::Serialize(json& out) const
{
	Component::Serialize(out["BoxCollider"]);

	out["BoxCollider"] =
	{
		{ "Size",
		{ { "X", size.x },
		  { "Y", size.y } }},
		{ "Offset",
		{ { "X", m_Offset.x },
		  { "Y", m_Offset.y } }}
	};
}

void BoxCollider::Deserialize(json& in)
{
	Component::Deserialize(in);

	size.x = in["Size"]["X"];
	size.y = in["Size"]["Y"];

	m_Offset.x = in["Offset"]["X"];
	m_Offset.y = in["Offset"]["Y"];
}