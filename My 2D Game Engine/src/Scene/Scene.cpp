//#include "Scene.h"
#include "../src/GameObject/GameObject.h"
#include "../Log/Log.h"

#include "../Math/Math.h"

GameObject* Scene::CreateGameObject()
{
	WARN("Game Object created");
	m_GameObjects.push_back(std::make_shared<GameObject>(this));

	return m_GameObjects.back().get();
	//m_GameObjects.push_back(std::shared_ptr<GameObject>(new GameObject(this)));
}

GameObject* Scene::GetObjectWithID(int ID)
{
	for (auto object : m_GameObjects)
	{
		if (object->GetID() == ID)
			return object.get();
	}

	return nullptr;
}

void Scene::UpdateOnEditor(EditorCamera& camera)
{
	for (auto it : m_RenderObjects)
	{
		if (it->gameObject->IsActive() && it->IsEnabled())
		{
			Shader& shader = it->GetShader();
			shader.Bind();
			shader.SetMat3("camera", camera.GetCameraMatrix());

			it->Draw();
		}
	}
}

void Scene::RuntimeStart()
{
	m_PhysicsWorld = new b2World(m_Gravity);

	for (auto object : m_GameObjects)
	{
		std::cout << "Possible ID: " << (int)object.get() << std::endl;

		if (object->HasComponent<Rigidbody>())
		{
			Rigidbody* rigidbody = object->GetComponent<Rigidbody>();
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;

			Vector2f pos = object->transform->position;
			bodyDef.position = b2Vec2(pos.x, pos.y);

			float angle = ToRads(-object->transform->angle);
			bodyDef.angle = angle;
			bodyDef.fixedRotation = false;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);


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

			body->CreateFixture(&fixtureDef);

			rigidbody->body = body;
		}
		else if (object->HasComponent<BoxCollider>())
		{
			BoxCollider* collider = object->GetComponent<BoxCollider>();

			b2BodyDef groundBodyDef;
			Vector2f pos = object->transform->position;
			groundBodyDef.position.Set(pos.x, pos.y);

			float angle = ToRads(-object->transform->angle);
			groundBodyDef.angle = angle;

			b2Body* groundBody = m_PhysicsWorld->CreateBody(&groundBodyDef);
			b2PolygonShape groundBox;
			float hx = collider->size.x * object->transform->scale.x;
			float hy = collider->size.y * object->transform->scale.y;
			b2Vec2 center = b2Vec2(collider->offset.x, collider->offset.y);
			groundBox.SetAsBox(abs(hx), abs(hy));
			groundBody->CreateFixture(&groundBox, 0.0f);
		}
	}
}

void Scene::RuntimeStop()
{
	delete m_PhysicsWorld;
	m_PhysicsWorld = nullptr;
}

void Scene::UpdateOnRuntime()
{
	for (auto it : m_GameObjects)
	{
		if (it->IsActive())
			it->ComponentsUpdate();
	}

	end = std::chrono::steady_clock::now();
	int64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

	if (duration >= 15)
	{
		begin = std::chrono::steady_clock::now();
		end = begin;

		m_PhysicsWorld->Step(0.015f, 6, 2);

		for (auto it : m_GameObjects)
		{
			if (it->IsActive())
				it->ComponentsFixedUpdate();
		}
	}

	for (auto it : m_GameObjects)
	{
		if (it->HasComponent<Rigidbody>())
		{
			Rigidbody* rigidbody = it->GetComponent<Rigidbody>();
			b2Body* body = rigidbody->body;
			it->transform->position.x = body->GetPosition().x;
			it->transform->position.y = body->GetPosition().y;
			it->transform->angle = -ToDegrees(body->GetAngle());

		}
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
	for (auto it : m_GameObjects)
	{
		if (it->HasComponent<Camera>())
		{
			it->GetComponent<Camera>()->SetRatio(float(heigth) / float(width));
		}
	}
}

void Scene::Render()
{
	Camera* mainCamera = nullptr;
	for (auto it : m_GameObjects)
	{
		if (it->HasComponent<Camera>())
		{
			mainCamera = it->GetComponent<Camera>();
		}
	}

	if (mainCamera != nullptr)
	{
		Color color = mainCamera->GetBackgroundColor();
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto it : m_RenderObjects)
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

void Scene::SaveGameObjectsData() // think about another solution to this problem
{
	for (auto it : m_GameObjectsData)
	{

		if (typeid(*(std::get<0>(it))) == typeid(Transform))
		{
			Transform* transform_1 = static_cast<Transform*>(std::get<0>(it).get());
			Transform* transform_2 = static_cast<Transform*>(std::get<1>(it).get());
			*transform_1 = *transform_2;
		}
		else if (typeid(*(std::get<0>(it))) == typeid(SpriteRenderer))
		{
			SpriteRenderer* spriteRenderer_1 = static_cast<SpriteRenderer*>(std::get<0>(it).get());
			SpriteRenderer* spriteRenderer_2 = static_cast<SpriteRenderer*>(std::get<1>(it).get());
			*spriteRenderer_1 = *spriteRenderer_2;
		}
	}
}

void Scene::ApplySavedData()
{
	for (auto it : m_GameObjectsData)
	{
		if (typeid(*(std::get<0>(it))) == typeid(Transform))
		{
			Transform* transform_1 = static_cast<Transform*>(std::get<0>(it).get());
			Transform* transform_2 = static_cast<Transform*>(std::get<1>(it).get());
			*transform_2 = *transform_1;
		}
		else if (typeid(*(std::get<0>(it))) == typeid(SpriteRenderer))
		{
			SpriteRenderer* spriteRenderer_1 = static_cast<SpriteRenderer*>(std::get<0>(it).get());
			SpriteRenderer* spriteRenderer_2 = static_cast<SpriteRenderer*>(std::get<1>(it).get());
			*spriteRenderer_2 = *spriteRenderer_1;
		}
	}
}