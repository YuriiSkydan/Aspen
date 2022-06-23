#include "Script.h"


void Script::SetName(const std::string& name)
{
	m_Name = name;
}

Script::Script(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{
}