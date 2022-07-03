#pragma once
#include "../Components/Transform.h"
#include "../Components/SpriteRenderer.h"
#include "../Log/Log.h"
#include "../Components/Script.h"

#include <string>
#include <iostream>

#define AllComponents Transform, SpriteRenderer, Camera, Rigidbody, BoxCollider, CircleCollider

class Scene;

class ASPEN GameObject
{
private:
	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<Script*> m_Scripts;
	Scene* m_Scene = nullptr;

	char m_Name[20];
	bool m_IsActive = true;
	unsigned int m_ID;

	inline static unsigned int s_Objects = 0;

	friend class Inspector;
	friend class Scene;
	friend class SceneSerializer;
public:
	Transform* transform;

private:
	void ComponentsUpdateOnEditor();

	void ComponentsAwake();
	void ComponentsStart();
	void ComponentsUpdate();
	void ComponentsFixedUpdate();
	void ComponentsLateUpdate();

	void AddScript(Script* script);

	template<typename... Components>
	void CopyComponents(const GameObject& other)
	{
		([&]()
			{
				if (other.HasComponent<Components>())
				{
					Components* component = AddComponent<Components>();
					*component = *(other.GetComponent<Components>());
				}
			}(), ...);
	}

public:
	GameObject(Scene* scene);
	GameObject(Scene* scene, const GameObject& other);

	GameObject(const GameObject& other) = delete;
	const GameObject& operator=(const GameObject& other) = delete;

	void SetName(const char* newName);
	const char* GetName() const { return m_Name; }
	unsigned int GetID() const { return m_ID; }

	bool IsActive() { return m_IsActive; }
	void SetActive(bool active) { m_IsActive = active; }

	std::vector<std::unique_ptr<Component>>& GetComponents() { return m_Components; }
	std::vector<Script*>& GetScripts() { return m_Scripts; }

	template<typename T>
	T* AddComponent();

	template<typename T>
	void RemoveComponent() // method isn't finished
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
	bool HasComponent() const;

	template<typename T>
	T* GetComponent() const;

	~GameObject();
};

#pragma region GameObject

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

		//m_Scene->OnComponentAdded<T>(newComponent);
		m_Components.push_back(std::move(newComponent));

		return returnComponent;
	}

	return nullptr;
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

#pragma endregion

#pragma region Component 

template<typename T>
T* Component::GetComponent()
{
	return gameObject->GetComponent<T>();
}

template<typename T>
bool Component::HasComponent()
{
	return gameObject->HasComponent<T>();
}

#pragma endregion