#include "Input.h"
#include "../Engine/Engine.h"

void Input::Init()
{
	GLFWwindow* window = Engine::Get().GetWindow().GetNativeWindow();
	//glfwSetKeyCallback(window, Input::KeyPressCallback);
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
	if(action == GLFW_REPEAT)
	{
		std::cout << "REPEAT EVENT!!!\n";
		s_InputBuffer.insert(key);
	}

	if (action == GLFW_RELEASE)
		s_InputBuffer.erase(key);
}

void Input::ScrollCallback(GLFWwindow* window, double xpos, double ypos)
{
	
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
	int state = GetKeyState(key);
	if (state == GLFW_PRESS)
	{
		if (s_InputBuffer.find(key) != s_InputBuffer.end())
		{
			std::cout << "Buton is here!!!\n";
			return false;
		}

		return true;
	}

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
