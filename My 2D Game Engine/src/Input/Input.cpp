#include "Input.h"
#include "../Engine/Engine.h"

bool Input::IsKeyPressed(KeyCode key)
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	int state = glfwGetKey(window, key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	int state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

Vector2f Input::GetMousePosition()
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return { (float)xPos, (float)yPos };
}

float Input::GetMouseX()
{
	return GetMousePosition().x;
}

float Input::GetMouseY()
{
	return GetMousePosition().y;
}
