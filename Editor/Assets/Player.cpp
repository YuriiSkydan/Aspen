#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Input/Input.h"

class Player : public Script
{
private:
	SpriteRenderer* renderer;
	Rigidbody* rigidbody;
	float testFloat;

public:
	void Start() override
	{
		std::cout << gameObject->GetName() << std::endl;
		if (gameObject->HasComponent<SpriteRenderer>())
			renderer = gameObject->GetComponent<SpriteRenderer>();

		rigidbody = GetComponent<Rigidbody>();
	}

	void Update() override
	{
		if (Input::IsKeyPressed(Key::A))
			rigidbody->AddForce(Vector2f(-5.0f, 0.0f));

		if (Input::IsKeyPressed(Key::D))
			rigidbody->AddForce(Vector2f(5.0f, 0.0f));

		if (Input::GetKeyDown(Key::Space))
		{
			rigidbody->AddForce(Vector2f(0.0f, 0.1f), ForceMode::Impulse);
		}

		Vector2f pos = transform->position;

		if (Input::GetKeyDown(Key::Space))
			std::cout << "Space key is down 1!!!\n";

		if (Input::GetKeyDown(Key::Space))
			std::cout << "Space key is down 2!!!\n";
		//if (renderer != nullptr)
		//	renderer->SetColor(Color(pos.x - int(pos.x), pos.y - int(pos.y), transform->scale.x, 1.0f));

	}
};

extern "C"
{
	__declspec(dllexport) void Destroy(Script* script)
	{
		Player* player = dynamic_cast<Player*>(script);
		if (player != nullptr)
			delete player;
		else
			delete script;
	}

	__declspec(dllexport) Script* Create()
	{
		Player* player = new Player;
		return player;
	}
}