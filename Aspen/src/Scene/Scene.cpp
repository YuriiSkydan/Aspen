#include "Scene.h"
#include "../Math/Math.h"
#include "../Core/Time.h"
#include "../Renderer/Renderer.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/PolygonCollider.h"
#include "../Components/Rigidbody.h"
#include "../Physics/ContactListener.h"
#include "../Physics/Physics.h"
#include "EditorCamera.h"

using namespace std::string_literals;

void Scene::Copy(const Scene& other)
{
	m_Name = other.m_Name;

	m_Width = other.m_Width;
	m_Height = other.m_Height;

	std::cout << "Scene coping!!!\n" << m_Name << std::endl;

	m_GameObjects.resize(other.m_GameObjects.size());

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

void Scene::Reserve(unsigned int objects)
{
	m_GameObjects.reserve(objects);
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

	std::sort(m_RenderObjects.begin(), m_RenderObjects.end(),
		[](SpriteRenderer* a, SpriteRenderer* b)
		{
			return a->orderInLayer < b->orderInLayer;
		});

	Renderer::BeginScene(camera.GetCameraMatrix());
	//Render objects
	for (auto& renderObj : m_RenderObjects)
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
	m_ContactListener = std::make_unique<ContactListener>(this);
	Physics::CreateWorld();
	Physics::SetContactListener(m_ContactListener.get());

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->IsActive())
			m_GameObjects[i]->ComponentsAwake();
	}

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->IsActive())
			m_GameObjects[i]->ComponentsStart();
	}
}

void Scene::Stop()
{
	Physics::DestoryWorld();
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

		Physics::Update();

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
	for (auto& camera : m_Cameras)
	{
		if (camera->gameObject->IsActive())
		{
			if (camera->IsEnabled())
			{
				mainCamera = camera;
			}
		}
	}

	if (mainCamera != nullptr)
	{
		Color color = mainCamera->backgroundColor;
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);

	/*	auto renderObjects = GetComponentsOfType<SpriteRenderer>();

		std::sort(renderObjects.begin(), renderObjects.end(),
			[](SpriteRenderer* a, SpriteRenderer* b)
			{
				return a->orderInLayer < b->orderInLayer;
			});*/

		Renderer::BeginScene(mainCamera->GetCameraMatrix());

		for (auto& it : m_RenderObjects)
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
	std::ofstream fileStream("Assets/"s + m_Name + ".scene", std::ofstream::binary);

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

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		auto newGameObject = std::make_unique<GameObject>(this);
		newGameObject->Deserialize(in[std::to_string(i)]);
		m_GameObjects[i] = std::move(newGameObject);
		
		//move it somewhere else, temporarily solution to fix bug
		m_GameObjects[i]->m_ID = i;
	};
}

Scene::~Scene()
{
	std::cout << "Scene Destructor!!!\n";
}