#pragma once
#include <iostream>
#include <memory>

#include "Component.h"
#include "Trigger.h"
#include "Collision.h"

class Script;

class GameObject;
class Inspector;

class ASPEN Script : public Component
{
private:
	std::string m_Name;

	friend class GameObject;
	friend class Inspector;
	friend class ScriptDeleter;
private:
	void SetName(const std::string& name);
	const std::string GetName() const { return m_Name; }

public:
	Script() : Component(nullptr, nullptr)
	{ 
	}
	Script(GameObject* gameObject, Transform* transform);

	virtual	void OnCollisionEnter(Collision* collision) {}
	virtual void OnCollisionStay(Collision* collision) {}
	virtual void OnCollisionExit(Collision* collision) {}

	virtual void OnTriggerEnter(Trigger* trigger) {}
	virtual void OnTriggerStay(Trigger* trigger) {}
	virtual void OnTriggerExit(Trigger* trigger) {}
};