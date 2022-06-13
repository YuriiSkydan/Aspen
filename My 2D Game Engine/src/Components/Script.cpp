#include "Script.h"

void Script::SetCreateFunction(ScriptFunctionPtr function)
{
	m_CreateFunction = function;
}

Script::Script(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{
}