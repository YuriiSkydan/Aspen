#include "Collider.h"

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Trigger(gameObject, transform)
	, Collision(gameObject, transform)
	, Component(gameObject, transform)
{
}
