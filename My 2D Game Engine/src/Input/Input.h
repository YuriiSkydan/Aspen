#pragma once
#ifndef INPUT_H
#define INPUT_H
#include "../Math/Vector2.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

class Input
{
public:
	static bool IsKeyPressed(KeyCode key);
	
	static bool IsMouseButtonPressed(MouseCode button);
	static Vector2f GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
};

#endif