#pragma once
#include "Component.h"

class Trigger : virtual public Component
{
protected:
	bool m_IsTrigger = false;

protected:
	Trigger(GameObject* gameObject, Transform* transform);

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;

	virtual ~Trigger() = default;
};