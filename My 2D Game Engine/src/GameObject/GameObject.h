#pragma once

#include <list>
#include <string>
#include <iostream>
#include "../Components/Transform.h"
#include "../Components/SpriteRenderer.h"
#include "../Scene/Scene.h"
#include "../Log/Log.h"

class GameObject
{
private:
	friend class Inspector;
	friend class Scene;
	friend class Hierarchy;

	std::vector<std::shared_ptr<Component>> m_Components;
	Scene* m_Scene = nullptr;

	char m_Name[20];
	bool m_IsActive = true;
	static size_t s_Objects;
public:
	std::shared_ptr<Transform> transform;

private:
	void ComponentsUpdateOnEditor();

	void ComponentsAwake();
	void ComponentsStart();
	void ComponentsUpdate();
	void ComponentsFixedUpdate();
	void ComponentsLateUpdate();
	void UpdateComponentsGUI(); // delete later // maybe

public:
	GameObject() = default;
	GameObject(Scene* scene);

	template<typename T>
	void AddComponent()
	{
		if (std::is_base_of<Component, T>::value)
		{
			if (HasComponent<T>())
			{
				WARN("Component is already added");
				return;
			}

			std::shared_ptr<T> newComponent = std::make_shared<T>(this, transform.get());
			m_Components.push_back(newComponent);

			m_Scene->OnComponentAdded<T>(newComponent);
		}
	}

	template<typename T>
	bool HasComponent()
	{
		for (auto component : m_Components)
		{
			if (typeid(T) == typeid(*component))
				return true;
		}

		return false;
	}

	template<typename T>
	void RemoveComponent() // method isn't finished
	{
		if (HasComponent<T>())
		{
			std::cout << "Componet exist but you need to finish the method!!!\n";
		}
		else
		{
			std::cerr << "Component isn't added!!!\n";
		}
	}

	template<typename T>
	T* GetComponent()
	{
		T* component = nullptr;
		for (auto it : m_Components)
		{
			component = dynamic_cast<T*>(it.get());
			if (component != nullptr)
				return component;
		}

		return component;
	}

	void SetName(const char* newName);
	const char* GetName() const { return m_Name; }
	
	bool IsActive() { return m_IsActive; }
	void SetActive(bool active) { m_IsActive = active; }

	std::vector<std::shared_ptr<Component>>& GetComponents() { return m_Components; }

	~GameObject()
	{
		std::cout << "GameObject destructor!!!\n";
	}
};