#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Scene/Scene.h"
#include "../../Aspen/src/Input/Input.h"
#include "../../Aspen/src/Core/Time.h"

class MovingPlatform : public Script
{
private:
	Rigidbody* rigidbody;

	float m_MovingSpeed = 5.0f;

public:
	void Start() override
	{
		rigidbody = GetComponent<Rigidbody>();

	}

	void Update() override
	{
		if (Input::IsKeyPressed(Key::X))
			transform->position.x += 0.01f;
		else if (Input::IsKeyPressed(Key::Z))
			transform->position.x -= 0.01f;
	}

	void OnCollisionEnter(Collision* collision) override
	{

	}

	void OnTriggerEnter(Trigger* trigger) override
	{
		std::cout << "On Trigger Enter!!!\n";
	}
};

RegisterScript(MovingPlatform);