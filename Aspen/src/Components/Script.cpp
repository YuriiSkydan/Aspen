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