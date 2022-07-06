#include "CircleCollider.h"

CircleCollider::CircleCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{
}