#pragma once
#include <memory>
#include "box2d/b2_world.h"
#include "ContactListener.h"
#include "../Math/Vector2.h"

class ASPEN Physics
{
private:
	inline static std::unique_ptr<b2World> s_World;
	inline static std::unique_ptr<ContactListener> s_ContactListener;
	inline static b2Vec2 s_Gravity = b2Vec2(0.0f, -10.0f);

	friend class Scene;
	friend class Rigidbody;
	friend class Collider;
private:
	static void CreateWorld();
	static void Update();
	static void DestoryWorld();
	static b2Body* CreateBody(const b2BodyDef& bodyDef);
	static b2World* GetWorld() { return s_World.get(); }
	static void SetContactListener(ContactListener* contactListener);

public:
	static void SetGravity(const Vector2f& gravity);
};