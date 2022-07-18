#pragma once
#include "../Log/Log.h"
#include "../Components/Transform.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/PolygonCollider.h"
#include "../Components/Animator.h"
#include "../Components/Script.h"

#include <string>
#include <iostream>

#define AllComponents Transform, SpriteRenderer, Camera, Rigidbody, BoxCollider, CircleCollider, PolygonCollider, Animator

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
	void ComponentsAwake();
	void ComponentsStart();
	void ComponentsUpdate();
	void ComponentsFixedUpdate();
	void ComponentsLateUpdate();
	void ComponentsEnable();
	void ComponentsDisable();

	void AddScript(Script* script);

	template<typename... Components>
	void CopyComponents(const GameObject& gameObject)
	{
		([&]()
			{
				if (gameObject.HasComponent<Components>())
				{
					Components* component = AddComponent<Components>();
					*component = *(gameObject.GetComponent<Components>());
				}
			}(), ...);
	}

	void RemoveComponent(Component* component);

public:
	GameObject(Scene* scene);
	GameObject(Scene* scene, const GameObject& other);

	GameObject(const GameObject& other) = delete;
	const GameObject& operator=(const GameObject& other) = delete;

	void SetName(const char* newName);
	void SetActive(bool active);

	bool IsActive() { return m_IsActive; }
	const char* GetName() const { return m_Name; }
	unsigned int GetID() const { return m_ID; }
	Scene* GetScene() const { return m_Scene; }

	std::vector<std::unique_ptr<Component>>& GetComponents() { return m_Components; }
	std::vector<Script*>& GetScripts() { return m_Scripts; }

	//In Scene.h
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