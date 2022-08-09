#include "Collider.h"

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Trigger(gameObject, transform)
	, Collision(gameObject, transform)
	, Component(gameObject, transform)
{
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