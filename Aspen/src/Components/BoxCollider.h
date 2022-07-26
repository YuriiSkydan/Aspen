#pragma once
#include "Collider.h"

class ASPEN BoxCollider : public Collider
{
public:
	Vector2f size = { 0.5f, 0.5f };

public:
	BoxCollider(GameObject* gameObject, Transform* transform);
};