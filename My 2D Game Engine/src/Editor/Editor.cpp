#include "Editor.h"
#include "ImGuizmo.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

glm::vec3 Position = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 Projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
glm::mat4 tran = glm::mat4(1.0f);

Editor::Editor()
	: m_HierarchyPanel(m_SelectedObject), m_InspectorPanel(m_SelectedObject), m_EditorCamera(1080.0f / 1920.0f)
{
	//delete later
	m_ActiveScene = std::make_shared<Scene>();

	m_HierarchyPanel.SetScene(m_ActiveScene);
}

void Editor::Update()
{
	m_SceneFramebuffer.Bind();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1920, 1080);

	m_ActiveScene->UpdateOnEditor(m_EditorCamera);

	if (m_SceneState == SceneState::PLAY && !m_Pause)
		m_ActiveScene->UpdateOnRuntime();

	//m_SceneFramebuffer.Unbind();

	m_GameFramebuffer.Bind();

	glViewport(0, 0, 1920, 1080);

	m_ActiveScene->Render();

	m_GameFramebuffer.Unbind();
}

void Editor::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(" "))
		{
			ImGui::EndMenu();
		}

		int xPos = ImGui::GetWindowSize().x / 2;
		ImGui::SameLine(xPos - 100);
		if (m_SceneState == SceneState::EDIT)
		{
			if (ImGui::Button("Play", ImVec2(50, 18)))
			{
				m_SceneState = SceneState::PLAY;
				m_Pause = false;

				m_ActiveScene->SaveGameObjectsData();
				m_ActiveScene->RuntimeStart();
			}
		}
		else
		{
			if (ImGui::Button("Stop", ImVec2(50, 18)))
			{
				m_SceneState = SceneState::EDIT;
				m_Pause = false;

				m_ActiveScene->ApplySavedData();
				m_ActiveScene->RuntimeStop();
			}
		}

		ImGui::SameLine(xPos - 50);
		if (ImGui::Button("Pause", ImVec2(50, 18)))
			m_Pause = !m_Pause;


		ImGui::EndMenuBar();
	}
}

void Editor::ImGuiRender()
{
	DockSpaceBegin();

	MenuBar();

	ImGui::ShowDemoWindow();

	m_HierarchyPanel.ImGuiRender();
	m_InspectorPanel.ImGuiRender();
	//m_ProjectPanel.ImGuiRender();

	SceneWindow();
	GameWindow();

	DockSpaceEnd();
}

void Editor::GameWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Game");

	start = ImGui::GetCursorScreenPos();
	size = ImGui::GetWindowSize();

	m_ActiveScene->ViewportResize(size.x, size.y);

	ImVec2 cursorPos = ImGui::GetCursorPos();
	glViewport(0, 0, size.x, size.y);

	ImGui::GetWindowDrawList()->AddImage(
		(void*)m_GameFramebuffer.GetTexture(),
		ImVec2(ImGui::GetWindowPos()),
		ImVec2(ImGui::GetWindowPos().x + size.x, ImGui::GetWindowPos().y + size.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	ImGui::PopStyleVar();
}

void Editor::SceneWindow()
{
	ImGui::Begin("Scene");

	start = ImGui::GetCursorScreenPos();
	m_SceneWindowSize = ImGui::GetWindowSize();

	glm::mat4 matrix = glm::mat4(1.5f);
	Vector2f pos = m_EditorCamera.GetPosition();

//	std::cout << "Camera Position X: " << pos.x << "\tY: " << pos.y << std::endl;

	glm::mat4 CameraView = glm::lookAt(Position, Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	CameraView = glm::scale(CameraView, glm::vec3(50.0f, 50.0f, 50.0f));
	CameraView = glm::translate(CameraView, glm::vec3(pos.x, 0.0f, pos.y));

	float ratio = float(m_SceneWindowSize.x) / float(m_SceneWindowSize.y);
	Projection = glm::ortho(float(-m_SceneWindowSize.x) / 2.0f, float(m_SceneWindowSize.x) / 2.0f,
		float(-m_SceneWindowSize.y) / 2.0f, float(m_SceneWindowSize.y) / 2.0f, -10.0f, 1000.0f);

	//ImGuizmo::Manipulate(,);
	ImGuizmo::DrawGrid((float*)&CameraView, (float*)&Projection,
		(float*)&matrix, 20.25);

	ImGuizmo::SetOrthographic(true);
	ImVec2 cursorPos = ImGui::GetCursorPos();
	glViewport(0, 0, m_SceneWindowSize.x, m_SceneWindowSize.y);
	ImGui::GetWindowDrawList()->AddImage(
		(void*)m_SceneFramebuffer.GetTexture(),
		ImVec2(ImGui::GetWindowPos()),
		ImVec2(ImGui::GetWindowPos().x + m_SceneWindowSize.x, ImGui::GetWindowPos().y + m_SceneWindowSize.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, m_SceneWindowSize.x, m_SceneWindowSize.y);

	//ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(Projection),
	//	(ImGuizmo::ROTATE_Z), ImGuizmo::LOCAL, glm::value_ptr(tran), nullptr, nullptr);

	m_EditorCamera.SetRatio(m_SceneWindowSize.y / m_SceneWindowSize.x);
	if (ImGui::IsWindowHovered())
		m_EditorCamera.Update();

	ImGui::RadioButton("Translate", true);

	ImGui::End();
}

void Editor::DockSpaceBegin()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("DockSpace", NULL, windowFlags);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0));
	}
}

void Editor::DockSpaceEnd()
{
	ImGui::End();
}
