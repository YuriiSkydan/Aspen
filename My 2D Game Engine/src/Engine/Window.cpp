#include "Window.h"
#include "../Log/Log.h"

Window::Window(const WindowData& data)
{
	Init(data);
}

void Window::Init(const WindowData& data)
{
	m_Data.title = data.title;
	m_Data.width = data.width;
	m_Data.height = data.height;

	if (!glfwInit())
	{
		ERROR("Failed to initialize glfw!!!");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), NULL, NULL);;
	
	if (m_Window == nullptr)
	{
		ERROR("Failed to create window!!!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGL())
	{
		ERROR("Failed to load GL!!!");
		glfwTerminate();
		return;
	}
}

std::unique_ptr<Window> Window::Create(const WindowData& data)
{
	return std::unique_ptr<Window>(std::make_unique<Window>(data));
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_VSync = enabled;
}

bool Window::IsVSync() const
{
	return m_VSync;
}
