#include <algorithm>
#include "Input.h"
#include "../Engine/Engine.h"

void Input::Init()
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	glfwSetKeyCallback(window, Input::KeyPressCallback);
	glfwSetScrollCallback(window, Input::ScrollCallback);
}

int Input::GetKeyState(KeyCode key)
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	int state = glfwGetKey(window, key);
	return state;
}

void Input::KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		s_KeyPressed.insert({ key, 0 });
	else if (action == GLFW_RELEASE)
		s_KeyPressed.erase(key);
}

void Input::ScrollCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void Input::FrameStart()
{
	for (auto it = s_KeyPressed.begin(); it != s_KeyPressed.end(); ++it)
		it->second++;
}

bool Input::IsKeyPressed(KeyCode key)
{
	int state = GetKeyState(key);
	return ((state == GLFW_PRESS) || (state == GLFW_REPEAT));
}

bool Input::IsKeyReleased(KeyCode key)
{
	int state = GetKeyState(key);
	return (state == GLFW_RELEASE);
}

bool Input::GetKeyUp(KeyCode key)
{
	return false;
}

bool Input::GetKeyDown(KeyCode key)
{
	if (s_KeyPressed.find(key) != s_KeyPressed.end() && s_KeyPressed[key] <= 1)
		return true;

	return false;
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
