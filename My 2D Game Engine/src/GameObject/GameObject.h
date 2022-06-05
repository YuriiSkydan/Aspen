#pragma once
#include "../Components/Transform.h"
#include "../Components/SpriteRenderer.h"
#include "../Scene/Scene.h"
#include "../Log/Log.h"

#include <list>
#include <string>
#include <iostream>

#define AllComponents Transform, SpriteRenderer, Camera, Rigidbody, BoxCollider, CircleCollider

class GameObject
{
private:
	std::vector<std::unique_ptr<Component>> m_Components;
	Scene* m_Scene = nullptr;

	char m_Name[20];
	bool m_IsActive = true;
	unsigned int m_ID;

	static unsigned int s_Objects;

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
	GameObject(const GameObject& other);

	void SetName(const char* newName);
	const char* GetName() const { return m_Name; }
	unsigned int GetID() const { return m_ID; }

	bool IsActive() { return m_IsActive; }
	void SetActive(bool active) { m_IsActive = active; }

	std::vector<std::unique_ptr<Component>>& GetComponents() { return m_Components; }

	template<typename T>
	T* AddComponent()
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
			m_Components.push_back(std::move(newComponent));

			return returnComponent;
		}

		return nullptr;
	}

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
	bool HasComponent() const
	{
		for (auto& component : m_Components)
		{
			if (typeid(T) == typeid(*component))
				return true;
		}

		return false;
	}

	template<typename T>
	T* GetComponent() const
	{
		for (auto& it : m_Components)
		{
			T* component = dynamic_cast<T*>(it.get());
			if (component != nullptr)
				return component;
		}

		return nullptr;
	}

	~GameObject()
	{
		std::cout << "GameObject destructor!!!\n";
	}
};