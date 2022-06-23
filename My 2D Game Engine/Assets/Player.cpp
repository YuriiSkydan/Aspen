#define SCRIPT
#include "../src/Components/Component.cpp"
#include "../src/Components/Transform.h"
#include "../src/Components/Script.h"

//#include "../src/GameObject/GameObject.cpp"
#include <conio.h>

#ifdef DLL_BUILD
#define CLASS_API __declspec(dllexport)
#else
#define CLASS_API __declspec(dllimport)
#endif

class Player : public Script
{
public:
	void Update() override
	{
		transform->position.x += 0.1f;
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