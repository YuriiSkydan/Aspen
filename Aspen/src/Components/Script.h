#pragma once
#include "Trigger.h"
#include "Collision.h"

#define SerializedField
#define GenerateBody()

enum class VariableTypes { INT = 0, BOOL, FLOAT, DOUBLE };

class Property
{
private:
	void* m_Variable;
	VariableTypes m_Type;
	std::string m_Name;

public:
	Property(void* ptr, VariableTypes type, const std::string& name)
		: m_Variable(ptr), m_Type(type), m_Name(name)
	{ }

	void* GetVariable() const { return m_Variable; }
	VariableTypes GetType() const { return m_Type; }
	const std::string& GetName() const { return m_Name; }
};

class ASPEN Script : public Component
{
private:
	std::string m_Name = "None";

	friend class GameObject;
	friend class ScriptManager;
	friend class Inspector;
protected:
	std::vector<Property> m_Properties;

private:
	void SetName(const std::string& name);
	const std::string GetName() const { return m_Name; }
	const std::vector<Property>& GetProperties() const { return m_Properties; }

public:
	Script();
	Script(GameObject* gameObject, Transform* transform);

	virtual	void OnCollisionEnter(Collision* collision) {}
	virtual void OnCollisionStay(Collision* collision) {}
	virtual void OnCollisionExit(Collision* collision) {}

	virtual void OnTriggerEnter(Trigger* trigger) {}
	virtual void OnTriggerStay(Trigger* trigger) {}
	virtual void OnTriggerExit(Trigger* trigger) {}

	void Remove();

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};