#include "Engine.h"
#include "../Log/Log.h"
#include "../Input/Input.h"
#include "ImGuizmo.h"

#include <chrono>

Engine* Engine::s_Instance = nullptr;

void Engine::InitImGui()
{
	const char* glslVersion = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	//io.Fonts->AddFontFromFileTTF("Fonts/SpaceMono-Regular.ttf", 18);
}

void Engine::ImGuiBegin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Engine::ImGuiEnd()
{
	ImGuiIO& io = ImGui::GetIO();

	float windowWidth = m_Window->GetWidth();
	float windowHeight = m_Window->GetHeight();
	io.DisplaySize = ImVec2(windowWidth, windowHeight);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupCurrentContext);
	}
}

Engine::Engine()
{
	Log::Init(); // maybe move it somewhere else

	INFO("Engine Start");

	s_Instance = this;
	m_Window = std::make_unique<Window>();

	InitImGui();
	m_Editor = std::make_unique<Editor>();
}

void Engine::Run()
{
	while (m_Running)
	{
		auto start = std::chrono::high_resolution_clock::now();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGuiBegin();

		m_Editor->Update();
		
		m_Editor->ImGuiRender();

		ImGuiEnd();

		m_Window->Update();
		//change latter
		m_Running = !glfwWindowShouldClose(m_Window->GetNativeWindow());

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		std::string title = "Aspen " + std::to_string(1.0f / duration.count());
		glfwSetWindowTitle(m_Window->GetNativeWindow(), title.c_str());
	}
}

void Engine::Close()
{
	m_Running = false;
}
