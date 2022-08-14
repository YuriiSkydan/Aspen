#include "Scene.h"
#include "../Math/Math.h"
#include "../Core/Time.h"
#include "../Renderer/Renderer.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/PolygonCollider.h"
#include "../Components/Rigidbody.h"
#include "../Physics/ContactListener.h"
#include "EditorCamera.h"

#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_circle_shape.h"

void Scene::PhysicsWorldStart()
{
	for (auto& object : m_GameObjects)
	{
		Vector2f position = object->transform->position;
		float angle = ToRads(-object->transform->angle);

		b2BodyDef bodyDef;
		bodyDef.position = b2Vec2(position.x, position.y);
		bodyDef.angle = angle;

		b2Body* body = nullptr;
		if (object->HasComponent<Rigidbody>())
		{
			Rigidbody* rigidbody = object->GetComponent<Rigidbody>();

			if (rigidbody->IsEnabled())
			{
				bodyDef.type = b2BodyType(rigidbody->GetBodyType());
				bodyDef.gravityScale = rigidbody->GetGravityScale();
				bodyDef.fixedRotation = true;
				//bodyDef.fixedRotation = rigidbody->GetFixedRotation();
				bodyDef.linearDamping = rigidbody->GetLinearDrag();
				bodyDef.angularDamping = rigidbody->GetAngularDrag();

				//	bodyDef.allowSleep = false;

				body = m_PhysicsWorld->CreateBody(&bodyDef);

				rigidbody->SetBody(body);
			}
		}

		if (object->HasComponent<BoxCollider>())
		{
			BoxCollider* collider = object->GetComponent<BoxCollider>();

			if (collider->IsEnabled())
			{
				float sizeX = collider->size.x * object->transform->scale.x;
				float sizeY = collider->size.y * object->transform->scale.y;
				b2Vec2 center = b2Vec2(collider->offset.x, collider->offset.y);

				b2PolygonShape boxShape;
				boxShape.SetAsBox(abs(sizeX), abs(sizeY), center, 0);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = collider->material.dencity;
				fixtureDef.restitution = collider->material.restitution;
				fixtureDef.friction = collider->material.friction;
				fixtureDef.isSensor = collider->isTrigger;
				fixtureDef.userData.pointer = object->GetID();

				if (body == nullptr)
					body = m_PhysicsWorld->CreateBody(&bodyDef);

				body->CreateFixture(&fixtureDef);
			}
		}

		if (object->HasComponent<CircleCollider>())
		{
			CircleCollider* collider = object->GetComponent<CircleCollider>();

			if (collider->IsEnabled())
			{
				b2CircleShape circleShape;
				circleShape.m_radius = collider->radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = collider->material.dencity;
				fixtureDef.restitution = collider->material.restitution;
				fixtureDef.friction = collider->material.friction;
				fixtureDef.isSensor = collider->isTrigger;

				if (body == nullptr)
					body = m_PhysicsWorld->CreateBody(&bodyDef);

				body->CreateFixture(&fixtureDef);
			}
		}

		if (object->HasComponent<PolygonCollider>())
		{
			PolygonCollider* collider = object->GetComponent<PolygonCollider>();

			if (collider->IsEnabled())
			{
				b2PolygonShape shape;
				//m_PhysicsWorld->Shap

			}
		}

		if (body != nullptr)
		{
			if (object->HasComponent<Rigidbody>())
			{
				Rigidbody* rigidbody = object->GetComponent<Rigidbody>();

				b2MassData massData = body->GetMassData();
				massData.mass = rigidbody->GetMass();
				
				body->SetMassData(&massData);
				body->SetFixedRotation(rigidbody->GetFixedRotation());
			}
		}
	}
}

void Scene::Copy(const Scene& other)
{
	m_Name = other.m_Name;

	m_Width = other.m_Width;
	m_Height = other.m_Height;

	std::cout << "Scene coping!!!\n" << m_Name << std::endl;

	m_GameObjects.resize(other.m_GameObjects.size());
	m_Gravity = other.m_Gravity;

	for (size_t i = 0; i < other.m_GameObjects.size(); i++)
	{
		m_GameObjects[i] = std::make_unique<GameObject>(this, *other.m_GameObjects[i]);
	}
}

Scene::Scene()
{

}

GameObject* Scene::CreateGameObject()
{
	WARN("Game Object created");

	m_GameObjects.push_back(std::make_unique<GameObject>(this));
	return m_GameObjects.back().get();
}

GameObject* Scene::CreateGameObject(const std::string& name)
{
	m_GameObjects.push_back(std::make_unique<GameObject>(this));
	m_GameObjects.back()->SetName(name);
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

std::vector<GameObject*> Scene::GetObjectsWithTag(const Tag& tag)
{
	std::vector<GameObject*> objectsWithTag;

	for (auto& object : m_GameObjects)
	{
		if (object->GetTag() == tag)
			objectsWithTag.push_back(object.get());
	}

	return objectsWithTag;
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		if (it->get() == gameObject)
		{
			m_GameObjects.erase(it);
			break;
		}
	}
}

void Scene::UpdateOnEditor(EditorCamera& camera)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto renderObjects = GetComponentsOfType<SpriteRenderer>();
	std::sort(renderObjects.begin(), renderObjects.end(),
		[](SpriteRenderer* a, SpriteRenderer* b)
		{
			return a->orderInLayer < b->orderInLayer;
		});

	Renderer::BeginScene(camera.GetCameraMatrix());
	//Render objects
	for (auto& renderObj : renderObjects)
	{
		if (renderObj != nullptr)
		{
			if (renderObj->gameObject->IsActive() && renderObj->IsEnabled())
			{
				Renderer::Draw(renderObj);
			}
		}
	}

	Renderer::EndScene();
}

void Scene::Start()
{
	m_PhysicsWorld = std::make_unique<b2World>(m_Gravity);
	m_ContactListener = std::make_unique<ContactListener>(this);

	m_PhysicsWorld->SetContactListener(m_ContactListener.get());

	PhysicsWorldStart();

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->IsActive())
			m_GameObjects[i]->ComponentsStart();
	}

	/*for (auto& object : m_GameObjects)
{
	if (object->HasComponent<Rigidbody>())
	{
		Rigidbody* rigidbody = object->GetComponent<Rigidbody>();

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = rigidbody->material.friction;
		fixtureDef.restitution = rigidbody->material.restitution;

		b2PolygonShape boxShape;
		if (object->HasComponent<BoxCollider>())
		{
			BoxCollider* collider = object->GetComponent<BoxCollider>();

			float hx = collider->size.x * object->transform->scale.x;
			float hy = collider->size.y * object->transform->scale.y;
			b2Vec2 center = b2Vec2(collider->offset.x, collider->offset.y);
			boxShape.SetAsBox(abs(hx), abs(hy), center, 0);

			fixtureDef.shape = &boxShape;
		}

		b2CircleShape circleShape;
		if (object->HasComponent<CircleCollider>())
		{
			CircleCollider* collider = object->GetComponent<CircleCollider>();

			circleShape.m_radius = collider->radius;
			fixtureDef.shape = &circleShape;
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
		float angle = ToRads(-object->transform->angle);

		bodyDef.position = b2Vec2(pos.x, pos.y);
		bodyDef.angle = angle;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		float sizeX = collider->size.x * object->transform->scale.x;
		float sizeY = collider->size.y * object->transform->scale.y;
		b2Vec2 center = b2Vec2(collider->offset.x, collider->offset.y);
		b2PolygonShape box;
		box.SetAsBox(abs(sizeX), abs(sizeY), center, 0);

		body->CreateFixture(&box, 0.0f);
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

		circle->CreateFixture(&circle, 0.0f);
	}
}*/
}

void Scene::Stop()
{
	m_PhysicsWorld.reset();
	m_ContactListener.reset();
}

void Scene::Update()
{
	static double duration = 0;
	duration += Time::DeltaTime();

	//Fixed Update
	if (duration >= Time::FixedDeltaTime())
	{
		duration = 0;

		int32 velocityIterations = 6;
		int32 positionIterations = 2;
		m_PhysicsWorld->Step(Time::FixedDeltaTime(), velocityIterations, positionIterations);

		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			if (m_GameObjects[i]->IsActive())
				m_GameObjects[i]->ComponentsFixedUpdate();
		}
	}

	//Update
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->IsActive())
			m_GameObjects[i]->ComponentsUpdate();
	}

	//Late Update
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->IsActive())
			m_GameObjects[i]->ComponentsLateUpdate();
	}
}

void Scene::Resize(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;

	for (auto& object : m_GameObjects)
	{
		if (object->HasComponent<Camera>())
		{
			object->GetComponent<Camera>()->SetRatio(float(height) / float(width));
		}
	}
}

void Scene::Render()
{
	Camera* mainCamera = nullptr;
	for (auto& object : m_GameObjects)
	{
		if (object->IsActive())
		{
			if (object->HasComponent<Camera>())
			{
				Camera* camera = object->GetComponent<Camera>();
				if (camera->IsEnabled())
				{
					mainCamera = object->GetComponent<Camera>();
				}
			}
		}
	}

	if (mainCamera != nullptr)
	{
		Color color = mainCamera->backgroundColor;
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		auto renderObjects = GetComponentsOfType<SpriteRenderer>();

		std::sort(renderObjects.begin(), renderObjects.end(),
			[](SpriteRenderer* a, SpriteRenderer* b)
			{
				return a->orderInLayer < b->orderInLayer;
			});

		Renderer::BeginScene(mainCamera);

		for (auto& it : renderObjects)
		{
			if (it != nullptr)
			{
				if (it->gameObject->IsActive() && it->IsEnabled())
				{
					Renderer::Draw(it);
				}
			}
		}

		Renderer::EndScene();
	}
	else
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Scene::Serialize() const
{
	std::ofstream fileStream(m_Name + ".scene", std::ofstream::binary);

	if (!fileStream.is_open())
	{
		return;
		ERROR("Failed to open the file!!!");
	}

	json serializer =
	{
		{ "Scene", {
			{ "Name", m_Name},
			{ "Objects amount", m_GameObjects.size() },
			{ "Gravity",{{"X", m_Gravity.x}, {"Y", m_Gravity.y}}}
		}}
	};

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Serialize(serializer[std::to_string(i)]);
	}

	fileStream << serializer;

	fileStream.close();
}

void Scene::Deserialize(json& in)
{
	m_Name = in["Scene"]["Name"];
	m_GameObjects.resize(in["Scene"]["Objects amount"]);
	m_Gravity.x = in["Scene"]["Gravity"]["X"];
	m_Gravity.y = in["Scene"]["Gravity"]["Y"];

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		auto newGameObject = std::make_unique<GameObject>(this);
		newGameObject->Deserialize(in[std::to_string(i)]);
		m_GameObjects[i] = std::move(newGameObject);
	};
}

Scene::~Scene()
{
	std::cout << "Scene Destructor!!!\n";
}