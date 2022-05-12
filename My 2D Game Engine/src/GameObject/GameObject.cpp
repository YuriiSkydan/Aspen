#include "GameObject.h"

size_t GameObject::s_Objects = 0;

GameObject::GameObject(Scene* scene)
	:m_Scene(scene)
{
	transform = std::make_shared<Transform>(this); // fix bug probably leak of memory because of shared pointer
	m_Components.push_back(transform);
	m_Scene->OnComponentAdded<Transform>(transform);

	std::stringstream name;
	name << "GameObject" << s_Objects << '\0';
	name >> m_Name;

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
	for (auto it : m_Components)
	{
		if (it->IsEnabled())
			it->Awake();
	}
}

void GameObject::ComponentsStart()
{
	for (auto it : m_Components)
	{
		if (it->IsEnabled())
			it->Start();
	}
}

void GameObject::ComponentsUpdate()
{
	for (auto it : m_Components)
	{
		if (it->IsEnabled())
			it->Update();
	}
}

void GameObject::ComponentsFixedUpdate()
{
	for (auto it : m_Components)
	{
		if (it->IsEnabled())
			it->FixedUpdate();
	}
}

void GameObject::ComponentsLateUpdate()
{
	for (auto it : m_Components)
	{
		if (it->IsEnabled())
			it->LateUpdate();
	}
}

void GameObject::UpdateComponentsGUI()
{

}
