#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Scene/Scene.h"
#include "../../Aspen/src/Input/Input.h"
#include "../../Aspen/src/Core/Time.h"

class Player : public Script
{
private:
	const int widthRange = -50;
	
	int currentXPos = -25;
	int currentYPos = 25;

	float m_Speed = 5.0f;
	float m_JumpForce = 10.0f;

	float m_MovingDirection = 0.0f;

	Rigidbody* m_Rigidbody;
	SpriteRenderer* m_SpriteRenderer;
public:

	void Start() override
	{
		std::cout << "Player Start!!!\n";
		//if (gameObject->HasComponent<SpriteRenderer>())
		//	renderer = gameObject->GetComponent<SpriteRenderer>();

		m_Rigidbody = GetComponent<Rigidbody>();
		m_SpriteRenderer = GetComponent<SpriteRenderer>();
	}

	void Update() override
	{
		{
			//if (Input::IsKeyPressed(Key::Space))
			//{
			//	auto created = gameObject->GetScene()->CreateGameObject();
			//	SpriteRenderer* renderer = created->AddComponent<SpriteRenderer>();
			//	//created->AddComponent<Rigidbody>();
			//	created->transform->position.x = currentXPos;
			//	created->transform->position.y = currentYPos;

			//	currentXPos++;
			//	if (currentXPos >= 25)
			//	{
			//		currentYPos--;
			//		currentXPos = -25;
			//	}

			//	Color color;
			//	color.r = (rand() % 255) / 255.0f;
			//	color.g = (rand() % 255) / 255.0f;
			//	color.b = (rand() % 255) / 255.0f;
			//	color.a = 1.0f;

			//	renderer->SetColor(color);
			//}
		}

		m_MovingDirection = 0.0f;
		if (Input::IsKeyPressed(Key::D))
		{
			m_SpriteRenderer->flipX = false;
			m_MovingDirection = 1.0f;
		}

		if (Input::IsKeyPressed(Key::A))
		{
			m_SpriteRenderer->flipX = true;
			m_MovingDirection = -1.0f;
		}

		if (Input::GetKeyDown(Key::Space))
			std::cout << "Key Is Down!!!\n";
	}

	void FixedUpdate() override
	{
		if (Input::IsKeyPressed(Key::Space))
			m_Rigidbody->AddForce(Vector2f(0.0f, m_JumpForce), ForceMode::Impulse);

		Vector2f currentVelocity = m_Rigidbody->GetLinearVelocity();
		currentVelocity.x = m_MovingDirection * m_Speed;
		m_Rigidbody->SetLinearVelocity(currentVelocity);
	}

	void OnCollisionEnter(Collision* collision) override
	{

	}

	void OnTriggerEnter(Trigger* trigger) override
	{
		std::cout << "On Trigger Enter!!!\n";
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