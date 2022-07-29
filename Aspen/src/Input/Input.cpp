#include "Input.h"
#include "../Engine/Engine.h"

void Input::Init()
{
	//GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	//glfwSetKeyCallback(window, Input::KeyPressCallback);
	//glfwSetScrollCallback(window, Input::ScrollCallback);
}

int Input::GetKeyState(KeyCode key)
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	int state = glfwGetKey(window, key);
	return state;
}

void Input::KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "Pressed callback!!!\n";
	s_ThisFramePressed.insert(key);
}

void Input::ScrollCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void Input::FrameStart()
{
	s_PreviousFramePressed = std::move(s_ThisFramePressed);
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
	if (!IsKeyPressed(key))
		return false;

	if (s_PreviousFramePressed.find(key) != s_PreviousFramePressed.end())
		return false;

	return true;
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
