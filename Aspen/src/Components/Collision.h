#pragma once
#include "Component.h"

class Collision : virtual public Component
{
protected:
	Collision(GameObject* gameObject, Transform* transform);
	virtual ~Collision() = default;
};

