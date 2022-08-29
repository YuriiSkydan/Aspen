#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Components/AllComponents.h"
#include "../../Aspen/src/Scene/Scene.h"
#include "../../Aspen/src/Input/Input.h"
#include "../../Aspen/src/Core/Time.h"

class Player : public Script
{
private:
	SerializedField float m_Speed = 2.0f;
	float m_JumpForce = 4.0f;

	float m_MovingDirection = 0.0f;

	Rigidbody* m_Rigidbody;
	SpriteRenderer* m_SpriteRenderer;
	Animator* m_Animator;

	GameObject* m_Camera;
	std::string currentAnimation;
public:

	void Start() override
	{
		std::cout << "Player start!!!\n";
		m_Rigidbody = GetComponent<Rigidbody>();
		m_SpriteRenderer = GetComponent<SpriteRenderer>();
		m_Animator = GetComponent<Animator>();

		auto objects = gameObject->GetScene()->GetObjectsWithComponent<Camera>();

		m_Camera = objects[0];
	}

	int currentXPos = -50;
	int currentYPos = 50;
	void Update() override
	{
		//{
		//	if (Input::IsKeyPressed(Key::Space))
		//	{
		//		//gameObject->GetScene()->Reserve(100);
		//		for (size_t i = 0; i < 100; i++)
		//		{
		//			auto created = CreateGameObject();
		//			created->AddComponent<Rigidbody>();
		//			created->AddComponent<BoxCollider>()->material.restitution = 0.5f;
		//			SpriteRenderer* renderer = created->AddComponent<SpriteRenderer>();
		//			created->transform->position.x = currentXPos;
		//			created->transform->position.y = currentYPos;

		//			currentXPos++;
		//			if (currentXPos >= 50)
		//			{
		//				currentYPos--;
		//				currentXPos = -50;
		//			}

		//			Color color;
		//			color.r = (rand() % 255) / 255.0f;
		//			color.g = (rand() % 255) / 255.0f;
		//			color.b = (rand() % 255) / 255.0f;
		//			color.a = 1.0f;

		//			renderer->SetColor(color);
		//		}
		//	}
		//}
		//if (Input::IsMouseButtonPressed(Mouse::Button0))
		//{
		//	auto created = CreateGameObject();
		//	created->AddComponent<Rigidbody>();
		//	created->AddComponent<CircleCollider>();

		//	auto renderer =	created->AddComponent<SpriteRenderer>();
		//	renderer->SetSprite("Assets/Sprites/circle.png")

		//	Color color;
		//	color.r = (rand() % 255) / 255.0f;
		//	color.g = (rand() % 255) / 255.0f;
		//	color.b = (rand() % 255) / 255.0f;
		//	color.a = 1.0f;

		//	renderer->SetColor(color);
		//}

		m_MovingDirection = 0.0f;

		currentAnimation = "Idle";

		m_Camera->transform->position.x = transform->position.x;
		m_Camera->transform->position.y = transform->position.y + 0.25;

		if (Input::IsKeyPressed(Key::D))
		{
			currentAnimation = "Run";

			m_SpriteRenderer->flipX = false;
			m_MovingDirection = 1.0f;
		}

		if (Input::IsKeyPressed(Key::A))
		{
			currentAnimation = "Run";

			m_SpriteRenderer->flipX = true;
			m_MovingDirection = -1.0f;
		}

		if (Input::IsKeyPressed(Key::C))
		{
			currentAnimation = "Roll";

			if (m_Rigidbody->GetLinearVelocity().x < 1.0f)
				m_Rigidbody->AddForce(Vector2f(2.0f, 0.0f));
		}

		if (Input::GetKeyDown(Key::Space))
			std::cout << "Key is down!!!\n";

		m_Animator->PlayAnimation(currentAnimation);
	}

	void FixedUpdate() override
	{
		if (Input::IsKeyPressed(Key::Space) && m_Rigidbody->GetLinearVelocity().y == 0)
		{
			std::cout << "Jumping\n";
			m_Rigidbody->AddForce(Vector2f(0.0f, m_JumpForce), ForceMode::Impulse);
		}

		Vector2f currentVelocity = m_Rigidbody->GetLinearVelocity();
		currentVelocity.x = m_MovingDirection * m_Speed;
		m_Rigidbody->SetLinearVelocity(currentVelocity);
	}

	~Player()
	{
		std::cout << "Player Deleted!!!\n";
	}
};

//RegisterScript(Player);

extern "C"
{
	__declspec(dllexport) Script* Create()
	{
		std::cout << "Create Function!!!\n";
		return new Player();
	}
}