#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Components/AllComponents.h"
#include "../../Aspen/src/Scene/Scene.h"
#include "../../Aspen/src/Input/Input.h"
#include "../../Aspen/src/Core/Time.h"

class Player : public Script
{
	GenerateBody()
private:
	[[SerializedField]] float m_Speed = 2.0f;
	[[SerializedField]] float m_JumpForce = 0.1f;
	[[SerializedField]] bool m_TestBool = false;
	[[SerializedField]] int m_TestInt = 10;
	float m_MovingDirection = 0.0f;
	
	Rigidbody* m_Rigidbody;
	SpriteRenderer* m_SpriteRenderer;
	Animator* m_Animator;

	GameObject* m_Camera;
	std::string currentAnimation;

public:
	void Start() override
	{

		m_Rigidbody = GetComponent<Rigidbody>();
		m_SpriteRenderer = GetComponent<SpriteRenderer>();
		m_Animator = GetComponent<Animator>();

		auto objects = gameObject->GetScene()->GetObjectsWithComponent<Camera>();

		m_Camera = objects[0];
	}

	void Update() override
	{
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

		m_Animator->PlayAnimation(currentAnimation);
	}

	void FixedUpdate() override
	{
		if (Input::IsKeyPressed(Key::Space) && m_Rigidbody->GetLinearVelocity().y == 0)
		{
			m_Rigidbody->AddForce(Vector2f(0.0f, m_JumpForce), ForceMode::Impulse);
		}

		Vector2f currentVelocity = m_Rigidbody->GetLinearVelocity();
		currentVelocity.x = m_MovingDirection * m_Speed;
		m_Rigidbody->SetLinearVelocity(currentVelocity);
	}
	
	void OnCollisionEnter(Collision* collision) override
	{
		GameObject* other = collision->gameObject;
		if ((other->GetLayer() & LayerMask::GetMask("Enemy")))
		{
			auto component = collision->gameObject->GetComponent<SpriteRenderer>();
			component->SetColor(Color(0.3f, 0.2f, 0.5f, 1.0f));
		}
	}
};