#pragma once
#include "Collider.h"
#include "box2d/b2_polygon_shape.h"

class ASPEN BoxCollider : public Collider
{
private:
	b2PolygonShape m_Shape;

public:
	Vector2f size = { 0.5f, 0.5f };

private:
	void SetShape();

public:
	BoxCollider(GameObject* gameObject, Transform* transform);

	void Start() override;
	void Reset() override;

	void SetOffset(const Vector2f& offset) override;

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};