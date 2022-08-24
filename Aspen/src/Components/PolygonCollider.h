#pragma once
#include <vector>
#include "Collider.h"
#include "box2d/b2_math.h"

class ASPEN PolygonCollider : public Collider
{
private:
	std::vector<b2Vec2> m_Verticies;

public:
	PolygonCollider(GameObject* gameObject, Transform* transform);

	void Start() override;
};