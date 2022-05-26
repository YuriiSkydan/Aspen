//#include "src/GameObject.h"
#pragma once
#include "../Editor/EditorCamera.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Camera.h"
#include "../Physics.h"
#include "../box2d/b2_api.h" // maybe move it to the physics
#include "../box2d/b2_body.h"
#include "../box2d/b2_world.h"
#include "../box2d/b2_math.h"
#include "../box2d/b2_polygon_shape.h"
#include "../box2d/b2_fixture.h"
#include "../box2d/b2_circle_shape.h"

#include <chrono>
#include <map>

class Scene
{
private:
	std::string m_Name = "Scene";

	//when vector is resized object will be destory that why you use unique_ptr
	std::vector<std::unique_ptr<GameObject>> m_GameObjects;
	std::vector<SpriteRenderer*>             m_RenderObjects;

	std::unique_ptr<b2World> m_PhysicsWorld;
	b2Vec2 m_Gravity = b2Vec2(0.0f, -10.0f);

	unsigned int m_ViewportWidth = 0;
	unsigned int m_ViewportHeigth = 0;

	//change it later
	//std::vector<std::tuple<std::unique_ptr<Component>, std::shared_ptr<Component>>> m_GameObjectsData;

	friend class HierarchyPanel;
	friend class SceneSerializer;
private:
	//delete later
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

public:
	Scene() = default;

	GameObject* CreateGameObject();
	GameObject* GetObjectWithID(int ID);
	//void DestroyGameObject();

	void Start();
	void Pause(); // I'm not sure whether I will use this function
	void Stop();
	void Update();
	void Render();
	void UpdateOnEditor(EditorCamera& camera);
	void Resize(unsigned int width, unsigned int heigth);

	void SaveGameObjectsData();
	void ApplySavedData();

	template<typename T>
	void OnComponentAdded(std::unique_ptr<T>& component)
	{
		if (typeid(T) == typeid(SpriteRenderer))
			m_RenderObjects.emplace_back((SpriteRenderer*)component.get());

		//m_GameObjectsData.push_back({ std::make_unique<T>(*component), component });
	}

	template<typename T>
	void OnComponentRemoved(std::unique_ptr<T>& component)
	{

	}

	std::string GetName() const { return m_Name; }

	~Scene();
};