#include "GameObject.h"
#include "../ScriptManager.h"
#include "../Components/AllComponents.h"

void GameObject::RemoveComponent(Component* component)
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if (it->get() == component)
		{
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

	//I don't like this, maybe change it
	m_NewComponents.clear();
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

void GameObject::Serialize(json& out) const
{
	out =
	{
		{ "Name",  m_Name },
		{ "ID", m_ID },
		{ "IsActive", m_IsActive }
	};

	for (auto& component : m_Components)
		component->Serialize(out);
}

void GameObject::Deserialize(json& in)
{
	strcpy_s(m_Name, 20, std::string(in["Name"]).c_str());
	m_ID = in["ID"];
	m_IsActive = in["IsActive"];

	transform->Deserialize(in["Transform"]);
	
	DeserializeComponent<SpriteRenderer> (in, "SpriteRenderer");
	DeserializeComponent<BoxCollider>    (in, "BoxCollider");
	DeserializeComponent<CircleCollider> (in, "CircleCollider");
	DeserializeComponent<PolygonCollider>(in, "PolygonCollider");
	DeserializeComponent<Rigidbody>      (in, "Rigidbody");
	DeserializeComponent<Camera>         (in, "Camera");
	DeserializeComponent<AudioSource>    (in, "AudioSource");
	DeserializeComponent<AudioListener>  (in, "AudioListener");
	DeserializeComponent<Animator>       (in, "Animator");
}

GameObject::~GameObject()
{
	//s_Objects--;
	std::cout << "GameObject destructor!!!\n";

	for (auto& component : m_Components)
		m_Scene->OnComponentRemoved(component.get());
}