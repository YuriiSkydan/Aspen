#pragma once
#include "Component.h"

class Trigger : virtual public Component
{
public:
	bool isTrigger = false;

protected:
	Trigger(GameObject* gameObject, Transform* transform);

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;

	virtual ~Trigger() = default;
};