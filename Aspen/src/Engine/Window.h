#pragma once
#include <string>
#include <memory>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../Core/Core.h"

struct ASPEN WindowData
{
	std::string title;
	unsigned int width;
	unsigned int height;

	WindowData(const std::string& title = "Aspen", unsigned int width = 1920, unsigned int height = 1080)
		: title(title), width(width), height(height)
	{}
};

class ASPEN Window
{
public:
	

private:
	GLFWwindow* m_Window;
	WindowData m_Data;

	bool m_VSync = false;

private:
	void Init(const WindowData& data);
	void ShutDown();

public:
	Window(const WindowData& data = WindowData());

	void Update();

	GLFWwindow* GetNativeWindow() const { return m_Window; }

	void SetVSync(bool enabled);
	bool IsVSync() const;

	unsigned int GetWidth() const { return m_Data.width; };
	unsigned int GetHeight() const { return m_Data.height; }
};