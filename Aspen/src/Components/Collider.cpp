#include "Collider.h"
#include "../GameObject/GameObject.h"

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Trigger(gameObject, transform)
	, Collision(gameObject, transform)
	, Component(gameObject, transform)
{ }

void Collider::Reset() 
{
	offset = { 0.0f, 0.0f };
	material.dencity = 1.0f;
	material.friction = 0.4f;
	material.restitution = 0.0f;
}

void Collider::SetFixtureDef()
{
	m_FixtureDef.density = material.dencity;
	m_FixtureDef.restitution = material.restitution;
	m_FixtureDef.friction = material.friction;
	m_FixtureDef.isSensor = isTrigger;
	m_FixtureDef.userData.pointer = gameObject->GetID();
}

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