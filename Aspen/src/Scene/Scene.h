#pragma once
#include "../GameObject/GameObject.h"
#include "../Components/Camera.h"
#include "../Components/BoxCollider.h"
#include "../Components/Rigidbody.h"
#include "../Components/AudioListener.h"

#include "box2d/b2_world.h"

class EditorCamera;
class ContactListener;

class ASPEN Scene
{
private:
	std::string m_Name = "Sample Scene";
	unsigned int m_Width;
	unsigned int m_Height;

	std::vector<std::unique_ptr<GameObject>> m_GameObjects;
	//std::vector<SpriteRenderer*>             m_RenderObjects;

	std::unique_ptr<b2World> m_PhysicsWorld;
	std::unique_ptr<ContactListener> m_ContactListener;
	b2Vec2 m_Gravity = b2Vec2(0.0f, -10.0f);

	friend class HierarchyPanel;
private:
	void PhysicsWorldStart();
	void PhysicsWorldStop();

public:
	Scene();

	Scene(const Scene& other) = delete;
	const Scene& operator=(const Scene& other) = delete;

	//------------------------------------------------------------
	//Update Functions
	void Start();
	void Stop();
	void Update();
	void Render();

	void UpdateOnEditor(EditorCamera& camera);
	void Resize(unsigned int width, unsigned int height);

	void Copy(const Scene& other);

	GameObject* CreateGameObject();
	GameObject* CreateGameObject(const std::string& name);
	void DestroyGameObject(GameObject* gameObject);

	//------------------------------------------------------------
	//Getters
	GameObject* GetObjectWithID(int ID);
	std::vector<GameObject*> GetObjectsWithTag(const Tag& tag);

	std::string GetName() const { return m_Name; }
	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	
	//------------------------------------------------------------
	//Component operations
	template<typename T>
	std::vector<T*> GetComponentsOfType() const
	{
		std::vector<T*> components;
		for (auto& object : m_GameObjects)
		{
			T* component = object->GetComponent<T>();

			if (component != nullptr)
				components.push_back(component);
		}

		return components;
	}

	template<typename T>
	std::vector<GameObject*> GetObjectsWithComponent() const
	{
		std::vector<GameObject*> objects;
		for (auto& object : m_GameObjects)
		{
			if (object->HasComponent<T>())
				objects.push_back(object.get());
		}

		return objects;
	}

	template<typename T>
	void OnComponentAdded(std::unique_ptr<T>& component);

	template<typename T>
	void OnComponentRemoved(std::unique_ptr<T>& component);

	//------------------------------------------------------------
	//Serialization
	void Serialize() const;
	void Deserialize(json& in);

	~Scene();
};

//---------------------------------------------
//Scene Component Operations
template<typename T>
void Scene::OnComponentAdded(std::unique_ptr<T>& component)
{
	//if (typeid(T) == typeid(SpriteRenderer))
	//	m_RenderObjects.emplace_back((SpriteRenderer*)component.get());
	if (typeid(T) == typeid(Camera))
	{
		Camera* camera = (Camera*)(component.get());
		camera->SetRatio(float(m_Height) / float(m_Width));
	}
}

template<typename T>
void Scene::OnComponentRemoved(std::unique_ptr<T>& component)
{

}

//---------------------------------------------
//GameObject Component Operations
template<typename T>
T* GameObject::AddComponent()
{
	if (std::is_base_of<Component, T>::value)
	{
		for (auto& it : m_Components)
		{
			T* component = dynamic_cast<T*>(it.get());
			if (component != nullptr)
			{
				WARN("Component is already added");
				return component;
			}
		}

		auto newComponent = std::make_unique<T>(this, transform);
		T* returnComponent = newComponent.get();

		m_Scene->OnComponentAdded<T>(newComponent);
		m_NewComponents.push_back(newComponent.get());
		m_Components.push_back(std::move(newComponent));
		
		return returnComponent;
	}

	return nullptr;
}

template<typename T>
T* GameObject::AddComponentToParent()
{
	Transform* parent = transform->GetParent();
	if (parent != nullptr)
		return parent->gameObject->AddComponent<T>();
}

//template<typename T>
//T* GameObject::AddComponent()
//{
//	if (std::is_base_of<Script, T>::value)
//	{
//		auto newScript = std::make_unique<T>(this, transform);
//
//		Script* script = newScript.get();
//		m_Scripts.push_back(newScript.get());
//
//		auto returnScript = newScript.get();
//		m_Components.push_back(std::move(newScript));
//
//		return returnScript;
//	}
//}

template<typename T>
void GameObject::RemoveComponent()
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		T* component = dynamic_cast<T*>(it->get());
		if (component != nullptr)
		{
			m_Components.erase(it);
			return;
		}
	}

	WARN("Component doesn't exist.");
}

template<typename T>
void GameObject::RemoveComponentInParent()
{
	Transform* parent = transform->GetParent();
	if (parent != nullptr)
		parent->gameObject->RemoveComponent<T>();
}

template<typename T>
bool GameObject::HasComponent() const
{
	for (auto& component : m_Components)
	{
		if (typeid(T) == typeid(*component))
			return true;
	}

	return false;
}

template<typename T>
bool GameObject::HasComponentInParent() const
{
	Transform* parent = transform->GetParent();
	if (parent != nullptr)
		return parent->gameObject->HasComponent<T>();

	return false;
}

template<typename T>
T* GameObject::GetComponent() const
{
	for (auto& it : m_Components)
	{
		T* component = dynamic_cast<T*>(it.get());
		if (component != nullptr)
			return component;
	}

	return nullptr;
}

template<typename T>
T* GameObject::GetComponentInParent() const
{
	Transform* parent = transform->GetParent();
	if (parent != nullptr)
		return parent->gameObject->GetComponent<T>();
	
	return nullptr;
}

//---------------------------------------------
//Component Component Operations
template<typename T>
T* Component::GetComponent() const
{
	return gameObject->GetComponent<T>();
}

template<typename T>
T* Component::GetComponentInParent() const
{
	return gameObject->GetComponentInParent<T>();
}

template<typename T>
bool Component::HasComponent() const
{
	return gameObject->HasComponent<T>();
}

template<typename T>
bool Component::HasComponentInParent() const
{
	return gameObject->GetComponentInParent<T>();
}