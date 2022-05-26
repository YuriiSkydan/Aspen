#include "GameObject.h"

unsigned int GameObject::s_Objects = 0;

GameObject::GameObject(Scene* scene)
	:m_Scene(scene)
{
	auto newComponent = std::make_unique<Transform>(this);
	transform = newComponent.get();
	m_Components.push_back(std::move(newComponent));
	
	std::stringstream name;
	name << "GameObject" << s_Objects << '\0';
	name >> m_Name;

	m_ID = s_Objects;
	s_Objects++;
}

void GameObject::SetName(const char* newName)
{
	strcpy_s(m_Name, 20, newName);
}

void GameObject::ComponentsUpdateOnEditor()
{

}

void GameObject::ComponentsAwake()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->Awake();
	}
}

void GameObject::ComponentsStart()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->Start();
	}
}

void GameObject::ComponentsUpdate()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->Update();
	}
}

void GameObject::ComponentsFixedUpdate()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->FixedUpdate();
	}
}

void GameObject::ComponentsLateUpdate()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->LateUpdate();
	}
}

void GameObject::UpdateComponentsGUI()
{

}
