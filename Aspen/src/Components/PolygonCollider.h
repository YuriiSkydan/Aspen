#pragma once
#include <vector>
#include "Collider.h"
#include "box2d/b2_math.h"

class ASPEN PolygonCollider : public Collider
{
private:
	std::vector<b2Vec2> m_Vertices;

public:
	PolygonCollider(GameObject* gameObject, Transform* transform);

	void Start() override;

	void AddVertex(const Vector2f& vertexPos);

	std::vector<b2Vec2>& GetVertices() { return m_Vertices; }

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};