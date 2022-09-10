#include "CircleCollider.h"

void CircleCollider::SetShape()
{
	b2Vec2 center = b2Vec2(m_Offset.x, m_Offset.y);
	m_Shape.m_radius = m_Radius;
	m_Shape.m_p = center;
}

CircleCollider::CircleCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void CircleCollider::Start()
{
	m_FixtureDef.shape = &m_Shape;
	Collider::Start();
}

void CircleCollider::SetRadius(float radius)
{
	m_Radius = radius;
	ResetShape();
}

void CircleCollider::Serialize(json& out) const
{
	out["CircleCollider"] =
	{
		{ "Radius", m_Radius }
	};

	Collider::Serialize(out["CircleCollider"]);
}

void CircleCollider::Deserialize(json& in)
{
	m_Radius = in["Radius"];
	Collider::Deserialize(in);
}