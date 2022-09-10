#pragma once
#include <vector>
#include "Collider.h"
#include "box2d/b2_math.h"
#include "box2d/b2_polygon_shape.h"

class ASPEN PolygonCollider : public Collider
{
private:
	b2PolygonShape m_Shape;
	std::vector<b2Vec2> m_Vertices;

private:
	void SetShape() override;

public:
	PolygonCollider(GameObject* gameObject, Transform* transform);

	void Start() override;
	void AddVertex(const Vector2f& vertexPos);
	void RemoveVertex(const Vector2f& vertexPos);
	void RemoveVertex(const std::vector<b2Vec2>::const_iterator& vertex);

	//------------------------------------------------------
	//Setters
	void SetVerices(const std::vector<b2Vec2>& verticies);

	//------------------------------------------------------
	//Getters
	const std::vector<b2Vec2>& GetVertices() const { return m_Vertices; }

	//------------------------------------------------------
	//Serialization
	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};