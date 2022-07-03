#pragma once

#include "Window.h"
#include "../Scene/Scene.h"
#include "../Layer.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class ASPEN Engine
{
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Layer> m_Editor;

	bool m_Running = true;
	bool m_Minimized = false;
	float m_LastFrameTime = 0.0f;

	inline static Engine* s_Instance = nullptr;

	ImGuiContext* m_Context;
	//bool OnWindowClose(WindowCloseEvent& e);
	//bool OnWindowResize(WindowResizeEvent& e);

private:
	//Move it to seperate class
	void InitImGui()
	{
		const char* glslVersion = "#version 130";
		IMGUI_CHECKVERSION();

		m_Context = ImGui::CreateContext();

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

		ImGui_ImplGlfw_InitForOpenGL(Engine::Get().GetWindow().GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init(glslVersion);

		//io.Fonts->AddFontFromFileTTF("Fonts/SpaceMono-Regular.ttf", 18);
	}
	void ImGuiBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiEnd()
	{
		Window& window = Engine::Get().GetWindow();
		ImGuiIO& io = ImGui::GetIO();

		float windowWidth = window.GetWidth();
		float windowHeight = window.GetHeight();
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

public:
	Engine();

	void Run();
	void Close();

	Window& GetWindow() const { return *m_Window; }

	void AddEditor(std::unique_ptr<Layer> editor)
	{
		m_Editor = std::move(editor);
	}

	ImGuiContext* GetImGuiContext() const { return m_Context; }

	static void Set(Engine* engine);
	static Engine& Get() { return *s_Instance; }
};