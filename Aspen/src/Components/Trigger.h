#pragma once
#include "Component.h"

class Trigger : virtual public Component
{
public:
	bool isTrigger = false;

protected:
	Trigger(GameObject* gameObject, Transform* transform);
	~Trigger() = default;
};

