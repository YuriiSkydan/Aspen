#include "BoxCollider.h"
#include "Rigidbody.h"
#include "../GameObject/GameObject.h"

void BoxCollider::SetShape()
{
	float sizeX = m_Size.x * transform->scale.x;
	float sizeY = m_Size.y * transform->scale.y;
	b2Vec2 center = b2Vec2(m_Offset.x, m_Offset.y);

	m_Shape.SetAsBox(abs(sizeX), abs(sizeY), center, 0);
}

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void BoxCollider::Start()
{
	m_FixtureDef.shape = &m_Shape;
	Collider::Start();
}

void BoxCollider::Reset()
{
	Collider::Reset();
	m_Size = { 0.5f, 0.5f };
}

void BoxCollider::SetSize(const Vector2f& size)
{
	m_Size = size;

	if (m_Body != nullptr)
	{
		b2Fixture* fixture = m_Body->GetFixtureList();
		m_Body->DestroyFixture(fixture);
		
		SetShape();
		m_Body->CreateFixture(&m_FixtureDef);
	}
}

void BoxCollider::Serialize(json& out) const
{
	out["BoxCollider"] =
	{
		{ "Size",
		{ { "X", m_Size.x },
		  { "Y", m_Size.y } }},
		{ "Offset",
		{ { "X", m_Offset.x },
		  { "Y", m_Offset.y } }}
	};

	Collider::Serialize(out["BoxCollider"]);
}

void BoxCollider::Deserialize(json& in)
{
	m_Size.x = in["Size"]["X"];
	m_Size.y = in["Size"]["Y"];

	m_Offset.x = in["Offset"]["X"];
	m_Offset.y = in["Offset"]["Y"];

	Collider::Deserialize(in);
}