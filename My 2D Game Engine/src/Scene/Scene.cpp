//#include "Scene.h"
#include "../src/GameObject/GameObject.h"
#include "../Log/Log.h"
#include "../Math/Math.h"


Scene::Scene(const Scene& other)
{
	m_Name = other.m_Name;
	std::cout << "Scene coping!!!\n" << m_Name << std::endl;

	m_GameObjects.resize(other.m_GameObjects.size());
	m_Gravity = other.m_Gravity;

	for (size_t i = 0; i < other.m_GameObjects.size(); i++)
	{
		m_GameObjects[i] = std::make_unique<GameObject>(this, *other.m_GameObjects[i]);
	}
}

GameObject* Scene::CreateGameObject()
{
	WARN("Game Object created");

	m_GameObjects.push_back(std::make_unique<GameObject>(this));
	return m_GameObjects.back().get();
}

GameObject* Scene::GetObjectWithID(int ID)
{
	for (auto& object : m_GameObjects)
	{
		if (object->GetID() == ID)
			return object.get();
	}

	return nullptr;
}

void Scene::UpdateOnEditor(EditorCamera& camera)
{
	for (auto& renderObj : m_RenderObjects)
	{
		if (renderObj != nullptr)
		{
			if (renderObj->gameObject->IsActive() && renderObj->IsEnabled())
			{
				Shader& shader = renderObj->GetShader();
				shader.Bind();
				shader.SetMat3("camera", camera.GetCameraMatrix());

				renderObj->Draw();
			}
		}
	}
}

void Scene::Start()
{
	m_PhysicsWorld = std::make_unique<b2World>(m_Gravity);

	for (auto& object : m_GameObjects)
	{
		if (object->HasComponent<Rigidbody>())
		{
			Rigidbody* rigidbody = object->GetComponent<Rigidbody>();

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = rigidbody->material.friction;
			fixtureDef.restitution = rigidbody->material.restitution;

			b2PolygonShape shape;
			if (object->HasComponent<BoxCollider>())
			{
				BoxCollider* collider = object->GetComponent<BoxCollider>();

				float hx = collider->size.x * object->transform->scale.x;
				float hy = collider->size.y * object->transform->scale.y;
				b2Vec2 center = b2Vec2(collider->offset.x, collider->offset.y);
				shape.SetAsBox(abs(hx), abs(hy));

				fixtureDef.shape = &shape;
			}

			b2CircleShape circle;
			if (object->HasComponent<CircleCollider>())
			{
				CircleCollider* collider = object->GetComponent<CircleCollider>();

				circle.m_radius = collider->radius;
				fixtureDef.shape = &circle;
			}

			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;

			Vector2f pos = object->transform->position;
			float angle = ToRads(-object->transform->angle);

			bodyDef.position = b2Vec2(pos.x, pos.y);
			bodyDef.angle = angle;
			bodyDef.fixedRotation = false;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);

			rigidbody->body = body;
		}
		else if (object->HasComponent<BoxCollider>())
		{
			BoxCollider* collider = object->GetComponent<BoxCollider>();

			b2BodyDef bodyDef;
			Vector2f pos = object->transform->position;
			bodyDef.position.Set(pos.x, pos.y);

			float angle = ToRads(-object->transform->angle);
			bodyDef.angle = angle;

			b2Body* groundBody = m_PhysicsWorld->CreateBody(&bodyDef);
			b2PolygonShape groundBox;
			float hx = collider->size.x * object->transform->scale.x;
			float hy = collider->size.y * object->transform->scale.y;
			b2Vec2 center = b2Vec2(collider->offset.x, collider->offset.y);
			groundBox.SetAsBox(abs(hx), abs(hy));
			groundBody->CreateFixture(&groundBox, 0.0f);
		}
		else if (object->HasComponent<CircleCollider>())
		{
			/*CircleCollider* collider = object->GetComponent<CircleCollider>();

			b2BodyDef bodyDef;
			Vector2f pos = object->transform->position;
			bodyDef.position.Set(pos.x, pos.y);
			
			b2Body* circle = m_PhysicsWorld->CreateBody(&bodyDef);
			b2CircleShape circleBody;
			circleBody.m_radius = collider->radius;

			circle->CreateFixture(&circle, 0.0f);*/
		}
	}
}

void Scene::Stop()
{
	m_PhysicsWorld.reset();
}

void Scene::Update()
{
	for (auto& object : m_GameObjects)
	{
		if (object->IsActive())
			object->ComponentsUpdate();
	}

	//Want to make objects moveble in editor while game is running 
	//for (auto& object : m_GameObjects)
	//{
	//	if (object->HasComponent<Rigidbody>())
	//	{
	//		Rigidbody* rigidbody = object->GetComponent<Rigidbody>();
	//		//Transform* transform = object->transform;
	//		b2Body* body = rigidbody->body;
	//		//if(body->GetPosition() == object->transform->position)
	//		//body->SetTransform();
	//		object->transform->position = (float*)&body->GetPosition();
	//		object->transform->angle = -ToDegrees(body->GetAngle());
	//	}
	//}

	begin = std::chrono::steady_clock::now();
	int64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(begin - end).count();
	
	if (duration >= 15)
	{
		m_PhysicsWorld->Step(0.015f, 6, 2);
		end = std::chrono::steady_clock::now();
	}

	for (auto& object : m_GameObjects)
	{
		if (object->IsActive())
			object->ComponentsFixedUpdate();
	}

	//end = std::chrono::steady_clock::now();
	//float duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

	//if (m_Debug)
	//	std::cout << "Debug!!!";

	//if (m_Play)
	//	m_PhysicsWorld.Update(duration / 1000.0f);

	//begin = end;
	//end = std::chrono::steady_clock::now();
}

void Scene::Resize(unsigned int width, unsigned int heigth)
{
	for (auto& object : m_GameObjects)
	{
		if (object->HasComponent<Camera>())
		{
			object->GetComponent<Camera>()->SetRatio(float(heigth) / float(width));
		}
	}
}

void Scene::Render()
{
	Camera* mainCamera = nullptr;
	for (auto& object : m_GameObjects)
	{
		if (object->HasComponent<Camera>())
		{
			mainCamera = object->GetComponent<Camera>();
		}
	}

	if (mainCamera != nullptr)
	{
		Color color = mainCamera->backgroundColor;
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto& it : m_RenderObjects)
		{
			if (it != nullptr)
			{
				if (it->gameObject->IsActive() && it->IsEnabled())
				{
					Shader& shader = it->GetShader();
					shader.Bind();
					shader.SetMat3("camera", mainCamera->GetCameraMatrix());

					it->Draw();
				}
			}
		}
	}
}

Scene::~Scene()
{
	std::cout << "Scene Destructor!!!\n";
}
