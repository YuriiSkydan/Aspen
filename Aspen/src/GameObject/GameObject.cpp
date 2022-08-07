#include "GameObject.h"
#include "../ScriptManager.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/Rigidbody.h"

void GameObject::RemoveComponent(Component* component)
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if (it->get() == component)
		{
			m_Components.erase(it);

			//Maybe call this function 
			//m_Scene->OnComponentRemoved();
			break;
		}
	}
}

GameObject::GameObject(Scene* scene)
	:m_Scene(scene)
{
	m_ID = s_Objects;
	s_Objects++;

	auto newComponent = std::make_unique<Transform>(this);
	m_Components.push_back(std::move(newComponent));
	transform = (Transform*)m_Components.back().get();

	std::stringstream name;
	name << "GameObject" << s_Objects << '\0';
	name >> m_Name;
}

GameObject::GameObject(Scene* scene, const GameObject& other)
	:m_Scene(scene)
{
	auto newComponent = std::make_unique<Transform>(this);
	transform = newComponent.get();
	m_Components.push_back(std::move(newComponent));

	strcpy_s(m_Name, 20, other.m_Name);
	m_IsActive = other.m_IsActive;
	m_ID = other.m_ID;

	CopyComponents<AllComponents>(other);

	for (auto& it : other.m_Scripts)
	{
		auto& scripts = ScriptManager::Get().GetScripts();
		Script* newScript = scripts.find(it->GetName())->second->Create();

		newScript->SetName(it->GetName());
		newScript->gameObject = this;
		newScript->transform = transform;

		m_Components.push_back(std::unique_ptr<Component>(newScript));
		m_Scripts.push_back(newScript);
	}
}

void GameObject::SetName(const std::string& name)
{
	strcpy_s(m_Name, 20, name.data());
}

void GameObject::SetActive(bool active)
{
	if (m_IsActive == true && active == false)
	{
		m_IsActive = active;
		ComponentsDisable();
	}
	else if (m_IsActive == false && active == true)
	{
		m_IsActive = active;
		ComponentsEnable();
	}
}

void GameObject::SetTag(const Tag& tag)
{
	m_Tag = tag;
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

void GameObject::ComponentsEnable()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->OnEnable();
	}
}

void GameObject::ComponentsDisable()
{
	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->OnDisable();
	}
}

void GameObject::AddScript(Script* script)
{
	script->gameObject = this;
	script->transform = transform;

	m_Components.push_back(std::unique_ptr<Component>(script));
	m_Scripts.push_back(script);
}

GameObject::~GameObject()
{
	//s_Objects--;
	std::cout << "GameObject destructor!!!\n";
}
