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

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = material.dencity;
	fixtureDef.restitution = material.friction;
	fixtureDef.isSensor = isTrigger;

	if (body == nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		body = Physics::CreateBody(bodyDef);
	}
	
	body->CreateFixture(&fixtureDef);
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