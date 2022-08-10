#include "PolygonCollider.h"

PolygonCollider::PolygonCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }
