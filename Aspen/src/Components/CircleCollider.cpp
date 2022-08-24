#include "CircleCollider.h"
#include "Rigidbody.h"
#include "../Physics/Physics.h"
#include "../GameObject/GameObject.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"

CircleCollider::CircleCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void CircleCollider::Start()
{
	b2Body* body = nullptr;
	if (gameObject->HasComponent<Rigidbody>())
		body = gameObject->GetComponent<Rigidbody>()->GetBody();

	b2Vec2 center = b2Vec2(offset.x, offset.y);
	b2CircleShape circleShape;
	circleShape.m_radius = radius;

	if (body == nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		body = Physics::CreateBody(bodyDef);
	}
	
	SetFixtureDef();
	m_FixtureDef.shape = &circleShape;
	body->CreateFixture(&m_FixtureDef);
}

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