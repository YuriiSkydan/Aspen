#pragma once
#include "../GameObject/GameObject.h"
#include "../Components/Camera.h"
#include "../Components/BoxCollider.h"
#include "../Components/Rigidbody.h"
#include "../Components/AudioListener.h"

class EditorCamera;
class ContactListener;

class ASPEN Scene
{
private:
	std::string m_Name = "Sample Scene";
	unsigned int m_Width = 0;
	unsigned int m_Height = 0;

	std::vector<std::unique_ptr<GameObject>> m_GameObjects;
	std::vector<SpriteRenderer*>             m_RenderObjects;
	std::vector<Camera*>                     m_Cameras;

	friend class HierarchyPanel;
private:
	//void PhysicsWorldStart();
	//void PhysicsWorldStop();

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
	void Reserve(unsigned int objects);
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
	void OnComponentAdded(T* component);

	template<typename T>
	void OnComponentRemoved(T* component);

	//------------------------------------------------------------
	//Serialization
	void Serialize() const;
	void Deserialize(json& in);
	void Deserialize(std::string_view filepath);

	~Scene();
};

//---------------------------------------------
//Scene Component Operations
template<typename T>
void Scene::OnComponentAdded(T* component)
{
	if (typeid(T) == typeid(Camera))
	{
		Camera* camera = (Camera*)(component);
		camera->SetRatio(float(m_Height) / float(m_Width));

		m_Cameras.push_back(camera);
	}
	else if (typeid(T) == typeid(SpriteRenderer))
	{
		SpriteRenderer* spriteRenderer = (SpriteRenderer*)(component);
		m_RenderObjects.push_back(spriteRenderer);
	}
}

template<typename T>
void Scene::OnComponentRemoved(T* component)
{
	if (Camera* camera = dynamic_cast<Camera*>(component))
	{
		for (auto it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
		{
			if (*it == camera)
			{
				it = m_Cameras.erase(it);
				break;
			}
		}
	}
	else if (SpriteRenderer* spriteRenderer = dynamic_cast<SpriteRenderer*>(component))
	{
		for (auto it = m_RenderObjects.begin(); it != m_RenderObjects.end(); ++it)
		{
			if (*it == spriteRenderer)
			{
				it = m_RenderObjects.erase(it);
				break;
			}
		}
	}
}

//---------------------------------------------
//GameObject Component Operations
template<typename T>
T* GameObject::AddComponent()
{
	T* returnComponent = nullptr;

	if (std::is_base_of<Script, T>::value)
	{
		auto newScript = std::make_unique<T>(this, transform);
		returnComponent = newScript.get();
		m_NewComponents.push_back(newScript.get());
		m_Components.push_back(std::move(newScript));
	}
	else if (std::is_base_of<Component, T>::value)
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
		returnComponent = newComponent.get();
		m_NewComponents.push_back(newComponent.get());
		m_Components.push_back(std::move(newComponent));
	}

	if (returnComponent != nullptr)
	{
		m_Scene->OnComponentAdded<T>(returnComponent);
	}

	return returnComponent;
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
			m_Scene->OnComponentRemoved<T>(it);
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
	/*	if (typeid(T) == typeid(*component))
			return true;*/
		if (dynamic_cast<T*>(component.get()))
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
		if (typeid(T) == typeid(*it))
		{
			return dynamic_cast<T*>(it.get());
		}
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