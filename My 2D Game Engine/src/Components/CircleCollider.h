#pragma once
#include "Trigger.h"
#include "Collision.h"
#include "Material.h"

class CircleCollider : public Trigger, public Collision
{
public:
	float radius = 0.5f;
	Material material;

public:
	CircleCollider(GameObject* gameObject, Transform* transform);

	void UpdateGui() override;
};
