#pragma once
#include "../Math/Vector2.h"
#include "Collision.h"
#include "Trigger.h"
#include "Material.h"

class ASPEN Collider : public Collision, public Trigger
{
public:
	Vector2f offset { 0.0f, 0.0f};
	Material material;

protected:
	Collider(GameObject* gameObject, Transform* transform);
};