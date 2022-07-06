#pragma once
#include <vector>
#include "Collider.h"

class ASPEN PolygonCollider : public Collider
{
private:
	std::vector<Vector2f> m_Verticies;

public:
	PolygonCollider(GameObject* gameObject, Transform* transform);
};

