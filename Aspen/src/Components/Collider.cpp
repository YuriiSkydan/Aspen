#include "Collider.h"
#include "Rigidbody.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Physics.h"
#include "../Math/Math.h"

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Trigger(gameObject, transform)
	, Collision(gameObject, transform)
	, Component(gameObject, transform)
{ }

void Collider::Start()
{
	if (gameObject->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		m_Body = rigidbody->GetBody();
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
	m_Body->CreateFixture(&m_FixtureDef);
}

void Collider::Reset()
{
	m_Offset = { 0.0f, 0.0f };
	material.dencity = 1.0f;
	material.friction = 0.4f;
	material.restitution = 0.0f;
}

void Collider::ResetShape()
{
	if (m_Body != nullptr)
	{
		b2Fixture* fixture = m_Body->GetFixtureList();
		m_Body->DestroyFixture(fixture);

		SetShape();
		m_Body->CreateFixture(&m_FixtureDef);
	}
}

void Collider::ResetFixture()
{
	if (m_Body != nullptr)
	{
		b2Fixture* fixture = m_Body->GetFixtureList();
		m_Body->DestroyFixture(fixture);

		SetFixtureDef();
		m_Body->CreateFixture(&m_FixtureDef);
	}
}

void Collider::SetOffset(const Vector2f& offset)
{
	m_Offset = offset;
	ResetShape();
}

void Collider::SetIsTrigger(bool isTrigger)
{
	m_IsTrigger = isTrigger;
	ResetFixture();
}

void Collider::SetFixtureDef()
{
	m_FixtureDef.density = material.dencity;
	m_FixtureDef.restitution = material.restitution;
	m_FixtureDef.friction = material.friction;
	m_FixtureDef.isSensor = m_IsTrigger;
	m_FixtureDef.userData.pointer = gameObject->GetID();
}

void Collider::SetShape()
{ }

void Collider::Serialize(json& out) const
{
	Trigger::Serialize(out);
	Collision::Serialize(out);
}

void Collider::Deserialize(json& in)
{
	Trigger::Deserialize(in);
	Collision::Deserialize(in);
}