#include "Engine.h"
#include "../Log/Log.h"
#include "../Input/Input.h"
#include "../Core/Time.h"
#include "../Renderer/Renderer.h"

#include <chrono>

Engine::Engine()
{
	Log::Init(); // maybe move it somewhere else
	INFO("Engine Start");

	s_Instance = this;
	m_Window = std::make_unique<Window>();

	m_TextureLibrary = std::make_unique<TextureLibrary>();

	Renderer::Init();
	InitImGui();
	Input::Init();
}

void Engine::Run()
{
	while (m_Running)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		Time::FrameStart();
		Input::FrameStart();

		m_Editor->Update();

		ImGuiBegin();
		m_Editor->ImGuiRender();
		ImGuiEnd();

		m_Window->Update();
		Time::FrameEnd();

		//change latter
		m_Running = !glfwWindowShouldClose(m_Window->GetNativeWindow());
		
		std::string title = "Aspen " + std::to_string(1.0f / Time::DeltaTime());
		glfwSetWindowTitle(m_Window->GetNativeWindow(), title.c_str());
	}
}

void Engine::Close()
{
	m_Running = false;
}

void Engine::Set(Engine* engine)
{
	s_Instance = engine;
}