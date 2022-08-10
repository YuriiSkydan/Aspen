#include "CircleCollider.h"

CircleCollider::CircleCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void CircleCollider::Serialize(json& out) const
{
	Collider::Serialize(out["CircleCollider"]);

	out["CircleCollider"] =
	{
		{ "Radius", radius }
	};
}

void CircleCollider::Deserialize(json& in)
{
	Collider::Deserialize(in);

	radius = in["Radius"];
}