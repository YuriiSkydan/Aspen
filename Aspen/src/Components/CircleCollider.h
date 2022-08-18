#pragma once
#include "Collider.h"

class ASPEN CircleCollider : public Collider
{
public:
	float radius = 0.5f;

public:
	CircleCollider(GameObject* gameObject, Transform* transform);

	void Start() override;

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};