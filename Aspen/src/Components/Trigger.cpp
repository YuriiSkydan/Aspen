#include "Trigger.h"

Trigger::Trigger(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void Trigger::Serialize(json& out) const
{
	Component::Serialize(out);
}

void Trigger::Deserialize(json& in)
{
	Component::Deserialize(in);
}