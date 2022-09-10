#include "Trigger.h"

Trigger::Trigger(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void Trigger::Serialize(json& out) const
{
	out["IsTrigger"] =  m_IsTrigger;
	Component::Serialize(out);
}

void Trigger::Deserialize(json& in)
{
	m_IsTrigger = in["IsTrigger"];
	Component::Deserialize(in);
}