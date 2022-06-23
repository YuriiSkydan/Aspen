#pragma once
#include "Component.h"
#include <iostream>
#include <memory>

class Script;

class GameObject;
class Inspector;

class Script : public Component
{
private:
	std::string m_Name;

	friend GameObject;
	friend Inspector;

private:
	void SetName(const std::string& name);
	const std::string GetName() const { return m_Name; }

public:
	Script()
		:Component(nullptr, nullptr)
	{ }
	Script(GameObject* gameObject, Transform* transform);

	virtual	void OnCollisionEnter() {}
	virtual void OnCollisionStay() {}
	virtual void OnTriggerEnter() {}
	virtual void OnTriggerStay() {}
	virtual void OnTriggerExit() {}
};