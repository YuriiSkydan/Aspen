#include "Script.h"
#include "../GameObject/GameObject.h"

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

void Script::Remove()
{
	gameObject->RemoveComponent(this);
}

void Script::Serialize(json& out) const
{
	out["Script"]["Name"] = m_Name;

	for (auto& property : m_Properties)
	{
		void* variable = property.GetVariable();
		switch (property.GetType())
		{
		case VariableTypes::INT:
			out[property.GetName()] = *(int*)(variable);
			break;
		case VariableTypes::FLOAT:
			out[property.GetName()] = *(float*)(variable);
			break;
		case VariableTypes::BOOL:
			out[property.GetName()] = *(bool*)(variable);
			break;
		}
	}
}

void Script::Deserialize(json& in)
{
	for (auto& property : m_Properties)
	{
		if (in.find(property.GetName()) != in.end())
		{
			void* variable = property.GetVariable();
			switch (property.GetType())
			{
			case VariableTypes::INT:
				*(int*)(variable) = in[property.GetName()];
				break;
			case VariableTypes::FLOAT:
				*(float*)(variable) = in[property.GetName()];
				break;
			case VariableTypes::BOOL:
				*(bool*)(variable) = in[property.GetName()];
				break;
			}
		}
	}
}