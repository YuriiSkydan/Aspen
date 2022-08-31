#include "Script.h"

Script::Script() 
	: Component(nullptr, nullptr)
{ }

Script::Script(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void Script::SetName(const std::string& name)
{
	m_Name = name;
}

void Script::Serialize(json& out) const
{
	out["Script"]["Name"] = m_Name;
}

void Script::Deserialize(json& in)
{

}