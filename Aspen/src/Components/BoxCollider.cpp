#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Physics.h"
#include "../Math/Math.h"
#include "BoxCollider.h"
#include "Rigidbody.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void BoxCollider::Start()
{
	b2Body* body = nullptr;
	float mass = 1.0f;
	if (gameObject->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		body = rigidbody->GetBody();
		mass = rigidbody->GetMass();
	}
	else
	{
		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		bodyDef.angle = ToRads(-transform->angle);
		body = Physics::CreateBody(bodyDef);
	}

	float sizeX = size.x * transform->scale.x;
	float sizeY = size.y * transform->scale.y;
	b2Vec2 center = b2Vec2(offset.x, offset.y);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(abs(sizeX), abs(sizeY), center, 0);

	SetFixtureDef();
	m_FixtureDef.shape = &boxShape;
	body->CreateFixture(&m_FixtureDef);

	std::cout << "Mass: " << body->GetMass() << std::endl;
}

void BoxCollider::Reset()
{
	Collider::Reset();
	size = { 0.5f, 0.5f };
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
		{ { "X", offset.x },
		  { "Y", offset.y } }}
	};
}

void BoxCollider::Deserialize(json& in)
{
	Component::Deserialize(in);

	size.x = in["Size"]["X"];
	size.y = in["Size"]["Y"];

	offset.x = in["Offset"]["X"];
	offset.y = in["Offset"]["Y"];
}