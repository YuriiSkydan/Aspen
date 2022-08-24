#include "PolygonCollider.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "Rigidbody.h"
#include "../GameObject/GameObject.h"

PolygonCollider::PolygonCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{
	m_Verticies.push_back({ -0.5f, 0.0f });
	m_Verticies.push_back({  0.5f, 0.0f });
	m_Verticies.push_back({  0.0f, 0.5f });
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

	b2PolygonShape shape;
	shape.Set(m_Verticies.data(), m_Verticies.size());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = material.dencity;
	fixtureDef.restitution = material.restitution;
	fixtureDef.friction = material.friction;
	fixtureDef.isSensor = isTrigger;
	fixtureDef.userData.pointer = gameObject->GetID();

	body->CreateFixture(&fixtureDef);
}