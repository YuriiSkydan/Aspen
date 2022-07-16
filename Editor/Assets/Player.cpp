#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Scene/Scene.h"
#include "../../Aspen/src/Input/Input.h"
#include "../../Aspen/src/Core/Time.h"

class Player : public Script
{
private:
	SpriteRenderer* renderer;
	Rigidbody* rigidbody;
	float testFloat;

	float m_MovingSpeed = 5.0f;

public:
	void Start() override
	{
		if (gameObject->HasComponent<SpriteRenderer>())
			renderer = gameObject->GetComponent<SpriteRenderer>();

		rigidbody = GetComponent<Rigidbody>();
	}

	void Update() override
	{
		//if (Input::IsMouseButtonPressed(Mouse::Button0))
		//{
		//	auto created = gameObject->GetScene()->CreateGameObject();
		//	SpriteRenderer* renderer = created->AddComponent<SpriteRenderer>();
		//	//created->AddComponent<Rigidbody>();

		//	Color color;
		//	color.r = (rand() % 255) / 255.0f;
		//	color.g = (rand() % 255) / 255.0f;
		//	color.b = (rand() % 255) / 255.0f;
		//	color.a = 1.0f;

		//	renderer->SetColor(color);
		//}

		std::cout << "\nDelta time: " << Time::DeltaTime();
		
		if (Input::IsKeyPressed(Key::A))
			transform->position.x -= m_MovingSpeed * Time::DeltaTime();
		else if (Input::IsKeyPressed(Key::D))
			transform->position.x += m_MovingSpeed * Time::DeltaTime();
		
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

RegisterScript(Player);