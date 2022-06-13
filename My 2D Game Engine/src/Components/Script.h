#pragma once
#include "Component.h"

class Script;

typedef Script*(__stdcall* ScriptFunctionPtr)();

class Script : public Component
{
private:
	ScriptFunctionPtr m_CreateFunction;

	friend GameObject;
private:
	void SetCreateFunction(ScriptFunctionPtr function);
	Script* Create() const { return m_CreateFunction(); }

public:
	Script() = default;
	Script(GameObject* gameObject, Transform* transform);

	virtual	void OnCollisionEnter() {}
	virtual void OnCollisionStay() {}
	virtual void OnTriggerEnter() {}
	virtual void OnTriggerStay() {}
	virtual void OnTriggerExit() {}
};