#pragma once
#include <unordered_map>

#include "../Engine/Window.h"
#include "../Math/Vector2.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

class ASPEN Input
{
private:
	inline static std::unordered_map<KeyCode, int> s_KeyPressed;

	friend class Engine;
private:
	static int GetKeyState(KeyCode key);
	static void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xpos, double ypos);

	static void FrameStart();
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