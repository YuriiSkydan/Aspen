#pragma once
#include "../Scene/EditorCamera.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/Rigidbody.h"
#include "../Components/Camera.h"
#include "../GameObject/GameObject.h"

#include "box2d/b2_api.h" // maybe move it to the physics
#include "box2d/b2_world.h"
#include "box2d/b2_math.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_world_callbacks.h"

#include <chrono>
#include <map>
#include <list>

//Make a seperate file for it
class ContactListener : public b2ContactListener
{
private:
	Scene* m_ScenePtr;

private:
	void OnTriggerEnter(GameObject* gameObject);
	void OnTriggerStay(GameObject* gameObject);
	void OnTriggerExit(GameObject* gameObject);

	void OnCollisionEnter(GameObject* gameObject, GameObject* entered);
	void OnCollisionStay(GameObject* gameObject);
	void OnCollisionExit(GameObject* gameObject);

public:
	ContactListener(Scene* scene);
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
};


class ASPEN Scene
{
private:
	std::string m_Name = "Sample Scene";

	//when vector is resized object will be destory that why you use unique_ptr
	std::vector<std::unique_ptr<GameObject>> m_GameObjects;
	//std::vector<SpriteRenderer*>             m_RenderObjects;

	std::unique_ptr<b2World> m_PhysicsWorld;
	std::unique_ptr<ContactListener> m_ContactListener;
	b2Vec2 m_Gravity = b2Vec2(0.0f, -10.0f);

	unsigned int m_Width;
	unsigned int m_Height;

	friend class HierarchyPanel;
	friend class SceneSerializer;
	friend class ContactListener;
private:
	//delete later
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

private:
	void PhysicsWorldStart();
	void PhysicsWorldStop();

public:
	Scene() = default;

	Scene(const Scene& other) = delete;
	const Scene& operator=(const Scene& other) = delete;

	GameObject* CreateGameObject();
	GameObject* GetObjectWithID(int ID);

	void DestroyGameObject(GameObject* gameObject);
	//void DestroyGameObject();

	void Start();
	void Stop();

	void Pause(); // I'm not sure whether I will use this function
	void Resume();

	void Update();
	void Render();

	void UpdateOnEditor(EditorCamera& camera);
	void Resize(unsigned int width, unsigned int height);

	void Copy(const Scene& other);

	std::string GetName() const { return m_Name; }
	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }

	template<typename T>
	void OnComponentAdded(std::unique_ptr<T>& component);

	template<typename T>
	void OnComponentRemoved(std::unique_ptr<T>& component);

	template<typename T>
	std::vector<T*> GetObjectsWithComponent()
	{
		std::vector<T*> objectsComponent;
		for (auto& object : m_GameObjects)
		{
			T* component = object->GetComponent<T>();

			if (component != nullptr)
				objectsComponent.push_back(component);
		}

		return objectsComponent;
	}

	~Scene();
};

#pragma region Scene

template<typename T>
void Scene::OnComponentAdded(std::unique_ptr<T>& component)
{
	//if (typeid(T) == typeid(SpriteRenderer))
	//	m_RenderObjects.emplace_back((SpriteRenderer*)component.get());
	if (typeid(T) == typeid(Camera))
	{
		std::cout << "Camera is added!!!\n";
		std::cout << "Scene width: " << m_Width << std::endl;
		std::cout << "Scene height: " << m_Height << std::endl;
		Camera* camera = (Camera*)(component.get());
		camera->SetRatio(float(m_Height) / float(m_Width));
	}
}

template<typename T>
void Scene::OnComponentRemoved(std::unique_ptr<T>& component)
{

}

#pragma endregion

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

		m_Scene->OnComponentAdded<T>(newComponent);
		m_Components.push_back(std::move(newComponent));

		return returnComponent;
	}

	return nullptr;
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

