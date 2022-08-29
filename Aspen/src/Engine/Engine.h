#pragma once
#include "Window.h"
#include "../Layer.h"
#include "../Scene/SceneManager.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"

class ASPEN Engine
{
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Layer> m_Editor;
	std::unique_ptr<SceneManager> m_SceneManager;

	bool m_Running = true;
	bool m_Minimized = false;

	inline static Engine* s_Instance = nullptr;

	ImGuiContext* m_Context;
	//bool OnWindowClose(WindowCloseEvent& e);
	//bool OnWindowResize(WindowResizeEvent& e);

private:
	//Move it to seperate class
	void InitImGui()
	{
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

		io.Fonts->AddFontFromFileTTF("Fonts/Montserrat-Bold.ttf", 15);

		ImGui_ImplGlfw_InitForOpenGL(Engine::Get().GetWindow().GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
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
	void DestroyImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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
	static Engine& Get() { return *s_Instance; }

	~Engine();
};