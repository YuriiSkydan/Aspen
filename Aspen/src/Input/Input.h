#pragma once
#include <unordered_set>

#include "../Engine/Window.h"
#include "../Math/Vector2.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

class ASPEN Input
{
private:
	inline static std::unordered_set<int> s_InputBuffer;

private:
	static int GetKeyState(KeyCode key);
	static void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	static void Init();
	static bool IsKeyPressed(KeyCode key);
	static bool IsKeyReleased(KeyCode key);
	static bool GetKeyUp(KeyCode key);
	static bool GetKeyDown(KeyCode key);

	static bool IsMouseButtonPressed(MouseCode button);
	static Vector2f GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
};