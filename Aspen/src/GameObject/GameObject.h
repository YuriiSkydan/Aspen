#pragma once
#include "Tag.h"
#include "LayerMask.h"
#include "../Log/Log.h"
#include "../Components/Script.h"

class SpriteRenderer;
class Camera;
class Rigidbody;
class BoxCollider;
class CircleCollider;
class PolygonCollider;
class Animator;
class AudioSource;
class AudioListener;
class Scene;

#define AllComponents Transform, SpriteRenderer, Camera, Rigidbody, BoxCollider, CircleCollider, PolygonCollider, Animator, AudioSource, AudioListener

class ASPEN GameObject
{
private:
	inline static unsigned int s_Objects = 0;

	unsigned int m_ID;
	bool m_IsActive = true;
	char m_Name[40];
	
	LayerMask m_Layer;
	Scene* m_Scene = nullptr;
	Tag m_Tag;

	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<Component*> m_NewComponents;
	std::vector<Script*> m_Scripts;

	friend class Inspector;
	friend class Scene;
public:
	Transform* transform;

private:
	void ProcessNewComponents();
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

	//------------------------------------------------------------
	//Component operations
	//Realization in Scene.h
	template<typename T>
	T* AddComponent();

	template<typename T>
	T* AddComponentToParent();

	template<typename T>
	T* GetComponent() const;

	template<typename T>
	T* GetComponentInParent() const;

	template<typename T>
	void RemoveComponent();

	template<typename T>
	void RemoveComponentInParent();

	template<typename T>
	bool HasComponent() const;

	template<typename T>
	bool HasComponentInParent() const;

	//------------------------------------------------------------
	//Setters
	void SetLayer(const LayerMask& layer);
	void SetName(const std::string& name);
	void SetActive(bool active);
	void SetTag(const Tag& tag);

	//------------------------------------------------------------
	//Getters
	const LayerMask& GetLayer() const { return m_Layer; }
	const char* GetName() const { return m_Name; }
	unsigned int GetID() const { return m_ID; }
	const Tag& GetTag() const { return m_Tag; }
	Scene* GetScene() const { return m_Scene; }
	bool IsActive() const { return m_IsActive; }

	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_Components; }
	const std::vector<Script*>& GetScripts() const { return m_Scripts; }

	//------------------------------------------------------------
	//Serialization
	template<typename T>
	void DeserializeComponent(json& in, const char* componentName)
	{
		if (in.find(componentName) != in.end())
		{
			T* component = AddComponent<T>();
			component->Deserialize(in[componentName]);
		}
	}

	void Serialize(json& out) const;
	void Deserialize(json& in);

	~GameObject();
};