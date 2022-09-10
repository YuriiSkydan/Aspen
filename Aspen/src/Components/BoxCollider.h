#pragma once
#include "Collider.h"
#include "box2d/b2_polygon_shape.h"

class ASPEN BoxCollider : public Collider
{
private:
	b2PolygonShape m_Shape;
	Vector2f m_Size = { 0.5f, 0.5f };

private:
	void SetShape() override;

public:
	BoxCollider(GameObject* gameObject, Transform* transform);

	void Start() override;
	void Reset() override;

	void SetSize(const Vector2f& size);
	const Vector2f& GetSize() const { return m_Size; }

	//-------------------------------------------------
	//Serialization
	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};