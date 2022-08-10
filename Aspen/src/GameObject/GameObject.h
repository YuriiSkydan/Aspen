#pragma once
#include "../Log/Log.h"
#include "../Components/Transform.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/PolygonCollider.h"
#include "../Components/Animator.h"
#include "../Components/AudioSource.h"
#include "../Components/AudioListener.h"
#include "../Components/Script.h"
#include "../Components/Rigidbody.h"
#include "Tag.h"
#include "LayerMask.h"

#include <string>
#include <iostream>

//class AudioSource;
//class AudioListener;

#define AllComponents Transform, SpriteRenderer, Camera, Rigidbody, BoxCollider, CircleCollider, PolygonCollider, Animator, AudioSource, AudioListener

class Scene;

class ASPEN GameObject
{
private:
	inline static unsigned int s_Objects = 0;

	char m_Name[20];
	unsigned int m_ID;
	bool m_IsActive = true;
	Scene* m_Scene = nullptr;
	Tag m_Tag;

	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<Script*> m_Scripts;

	friend class Inspector;
	friend class Scene;
public:
	Transform* transform;

private:

#pragma region ComponentsUpdates
	void ComponentsAwake();
	void ComponentsStart();
	void ComponentsUpdate();
	void ComponentsFixedUpdate();
	void ComponentsLateUpdate();
	void ComponentsEnable();
	void ComponentsDisable();
#pragma endregion

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

#pragma region ComponentMethods
	//In Scene.h
	template<typename T>
	T* AddComponent();

	template<typename T>
	void RemoveComponent()
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
#pragma endregion

#pragma region Setters
	void SetName(const std::string& name);
	void SetActive(bool active);
	void SetTag(const Tag& tag);
#pragma endregion

#pragma region Getters
	const char* GetName() const { return m_Name; }
	unsigned int GetID() const { return m_ID; }
	const Tag& GetTag() const { return m_Tag; }
	Scene* GetScene() const { return m_Scene; }
	bool IsActive() const { return m_IsActive; }

	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_Components; }
	const std::vector<Script*>& GetScripts() const { return m_Scripts; }
#pragma endregion

	void Serialize(json& out) const;
	void Deserialize(json& in);

	~GameObject();
};