#pragma once

#include "Window.h"
#include "../Scene/Scene.h"
#include "../Editor/Editor.h"

class Engine
{
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Editor> m_Editor;

	bool m_Running = true;
	bool m_Minimized = false;
	float m_LastFrameTime = 0.0f;

	static Engine* s_Instance;

	//bool OnWindowClose(WindowCloseEvent& e);
	//bool OnWindowResize(WindowResizeEvent& e);
public:
	Engine();

	void Run();
	void Close();

	Window& GetWindow() const { return *m_Window; }

	static Engine& Get() { return *s_Instance; }
};