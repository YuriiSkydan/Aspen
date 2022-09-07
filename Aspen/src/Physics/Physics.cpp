#include "Physics.h"
#include "../Core/Time.h"

void Physics::CreateWorld()
{
	s_World = std::make_unique<b2World>(s_Gravity);
	s_ContactListener = std::make_unique<ContactListener>();
	s_World->SetContactListener(s_ContactListener.get());
}

void Physics::Update()
{
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	s_World->Step(Time::FixedDeltaTime(), velocityIterations, positionIterations);
}

void Physics::DestoryWorld()
{
	s_World.reset();
}

b2Body* Physics::CreateBody(const b2BodyDef& bodyDef)
{
	return s_World->CreateBody(&bodyDef);
}

void Physics::SetContactListener(ContactListener* contactListener)
{
	s_World->SetContactListener(contactListener);
}

void Physics::SetGravity(const Vector2f& gravity)
{
	s_Gravity.x = gravity.x;
	s_Gravity.y = gravity.y;
}