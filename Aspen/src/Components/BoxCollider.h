#pragma once
#include "../Math/Vector2.h"
#include "Trigger.h"
#include "Collision.h"
#include "Material.h"

class ASPEN BoxCollider : public Trigger, public Collision
{
public:
	Vector2f offset = { 0.0f, 0.0f };
	Vector2f size = { 0.5f, 0.5f };

	Material material;

public:
	BoxCollider(GameObject* gameObject, Transform* transform);

	void UpdateGui() override;
};