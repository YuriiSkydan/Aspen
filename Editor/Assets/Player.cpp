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

public:
	Player()
	{
		std::cout << "Player Constructor!!!\n";
	}
	void Start() override
	{
		std::cout << "Player Start!!!\n";
		//if (gameObject->HasComponent<SpriteRenderer>())
		//	renderer = gameObject->GetComponent<SpriteRenderer>();

		//rigidbody = GetComponent<Rigidbody>();
	}

	void Update() override
	{
		if (Input::IsKeyPressed(Key::Space))
		{
			auto created = gameObject->GetScene()->CreateGameObject();
			SpriteRenderer* renderer = created->AddComponent<SpriteRenderer>();
			//created->AddComponent<Rigidbody>();
			created->transform->position.x = currentXPos;
			created->transform->position.y = currentYPos;

			currentXPos++;
			if (currentXPos >= 25)
			{
				currentYPos--;
				currentXPos = -25;
			}

			Color color;
			color.r = (rand() % 255) / 255.0f;
			color.g = (rand() % 255) / 255.0f;
			color.b = (rand() % 255) / 255.0f;
			color.a = 1.0f;

			renderer->SetColor(color);
		}
	}

	void FixedUpdate() override
	{

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