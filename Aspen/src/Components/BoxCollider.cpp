#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ 
}