#include "GameObject.h"
#include "../ScriptManager.h"
#include "../Components/AllComponents.h"

void GameObject::RemoveComponent(Component* component)
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if (it->get() == component)
		{
			if(Script* script = dynamic_cast<Script*>(component))
			{
				for(auto it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
				{
					if (*it == script)
					{
						it = m_Scripts.erase(it);
						break;
					}
				}
			}

			m_Scene->OnComponentRemoved(component);
			m_Components.erase(it);
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

void GameObject::SetLayer(const LayerMask& layer)
{
	m_Layer = layer;
}

void GameObject::SetName(const std::string& name)
{
	strcpy_s(m_Name, 40, name.data());
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

void GameObject::ProcessNewComponents()
{
	for (auto& component : m_NewComponents)
		component->Awake();

	for (auto& component : m_NewComponents)
		component->Start();

	m_NewComponents.clear();
}

void GameObject::ComponentsAwake()
{
	m_NewComponents.clear();

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
	ProcessNewComponents();

	for (auto& it : m_Components)
	{
		if (it->IsEnabled())
			it->Update();
	}
}

void GameObject::ComponentsFixedUpdate()
{
	ProcessNewComponents();

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
	m_NewComponents.push_back(script);
	m_Scripts.push_back(script);
}

void GameObject::DeserializeComponent(json& in)
{
	DeserializeComponent<PolygonCollider>(in, "PolygonCollider");
	DeserializeComponent<SpriteRenderer>(in, "SpriteRenderer");
	DeserializeComponent<CircleCollider>(in, "CircleCollider");
	DeserializeComponent<AudioListener>(in, "AudioListener");
	DeserializeComponent<AudioSource>(in, "AudioSource");
	DeserializeComponent<BoxCollider>(in, "BoxCollider");
	DeserializeComponent<Rigidbody>(in, "Rigidbody");
	DeserializeComponent<Animator>(in, "Animator");
	DeserializeComponent<Camera>(in, "Camera");

	if (in.find("Script") != in.end())
	{
		std::string scriptName = in["Script"]["Name"];
		auto& scripts = ScriptManager::Get().GetScripts();
		auto script = scripts.find(scriptName);
		Script* newComponent = script->second->Create();
		newComponent->SetName(scriptName);
		AddScript(newComponent);
	}
}

void GameObject::Serialize(json& out) const
{
	out =
	{
		{ "Name",  m_Name },
		{ "ID", m_ID },
		{ "IsActive", m_IsActive },
		{ "Components", m_Components.size() }
	};

	for (size_t i = 0; i < m_Components.size(); i++)
		m_Components[i]->Serialize(out[std::to_string(i)]);
}

void GameObject::Deserialize(json& in)
{
	strcpy_s(m_Name, 20, std::string(in["Name"]).c_str());
	m_ID = in["ID"];
	m_IsActive = in["IsActive"];

	size_t components = in["Components"];
	m_Components.reserve(components);

	transform->Deserialize(in[std::to_string(0)]["Transform"]);

	for (size_t i = 1; i < components; i++)
		DeserializeComponent(in[std::to_string(i)]);
}

GameObject::~GameObject()
{
	//s_Objects--;
	for (auto& component : m_Components)
		m_Scene->OnComponentRemoved(component.get());

	INFO("GameObject Destructor");
}