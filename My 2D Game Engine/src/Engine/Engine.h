#pragma once

#ifndef ENGINE_H
#define ENGINE_H

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

	//maybe move it to the editor // yeah think about it
	enum class Theme { Dark, Black, DarkNight, CorporateGray, Light };
	Theme m_CurrentTheme = Theme::Dark;

	//bool OnWindowClose(WindowCloseEvent& e);
	//bool OnWindowResize(WindowResizeEvent& e);
private:
	void InitImGui();
	void ImGuiBegin();
	void ImGuiEnd();
	void BlackStyle();
	void CorporateGreyStyle();
	void DarkStyle();
	void DarkNightStyle();
	void SetCurrentTheme();

	void ImGuiMainMenuBar();

public:
	Engine();

	void Run();
	void Close();

	Window& GetWindow() const { return *m_Window; }

	static Engine& Get() { return *s_Instance; }
};

#endif