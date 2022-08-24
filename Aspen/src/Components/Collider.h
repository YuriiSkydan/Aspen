#pragma once
#include "box2d/b2_fixture.h"
#include "../Math/Vector2.h"
#include "Collision.h"
#include "Trigger.h"
#include "Material.h"

class ASPEN Collider : public Collision, public Trigger
{
protected:
	b2FixtureDef m_FixtureDef;

public:
	Vector2f offset { 0.0f, 0.0f};
	Material material;

protected:
	Collider(GameObject* gameObject, Transform* transform);

	void Reset() override;

	void SetFixtureDef();

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};