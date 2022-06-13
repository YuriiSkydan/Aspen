#include "Engine.h"
#include "../Log/Log.h"
#include "../Input/Input.h"
#include "../ScriptManager.h"
#include "ImGuizmo.h"

#include <chrono>

Engine* Engine::s_Instance = nullptr;

Engine::Engine()
{
	Log::Init(); // maybe move it somewhere else

	INFO("Engine Start");

	s_Instance = this;
	m_Window = std::make_unique<Window>();

	m_Editor = std::make_unique<Editor>();
}

void Engine::Run()
{
	while (m_Running)
	{
		auto start = std::chrono::high_resolution_clock::now();

		glClear(GL_COLOR_BUFFER_BIT);

		m_Editor->Update();
		m_Editor->ImGuiRender();

		m_Window->Update();
		//change latter
		m_Running = !glfwWindowShouldClose(m_Window->GetNativeWindow());

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		std::string title = "Aspen " + std::to_string(1.0f / duration.count());
		glfwSetWindowTitle(m_Window->GetNativeWindow(), title.c_str());
	
		//Think about moving it to editor
		ScriptManager::GetInstance().Update();
	}
}

void Engine::Close()
{
	m_Running = false;
}
