#include "Editor.h"
#include "ImGuizmo.h"

#include "../Math/Math.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

#include "../Input/Input.h"

#include "../Renderer/Renderer.h" // delete later

glm::vec3 Position = glm::vec3(0.0f, 0.0f, 1.0f);
glm::mat4 Projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
glm::mat4 objectTransform = glm::mat4(1.0f);

Editor::Editor()
	: m_HierarchyPanel(m_SelectedObject), m_InspectorPanel(m_SelectedObject), m_EditorCamera(1080.0f / 1920.0f)
{
	glDebugMessageCallback(DebugMessageCallback, nullptr);
	glfwSetErrorCallback(ErrorCallback);
	//delete later
	m_ActiveScene = std::make_shared<Scene>();

	m_HierarchyPanel.SetScene(m_ActiveScene);
	
	m_SceneFramebuffer.Bind();
	m_SceneFramebuffer.AddColorAttachment(GL_RGBA8, GL_RGBA);
	m_SceneFramebuffer.AddColorAttachment(GL_R32I, GL_RED_INTEGER);
	m_SceneFramebuffer.DrawBuffers();

	m_GameFramebuffer.Bind();
	m_GameFramebuffer.AddColorAttachment(GL_RGBA8, GL_RGBA);
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

	Vector2f mPos = Input::GetMousePosition();
	int pData = m_SceneFramebuffer.ReadPixel(1, mPos.x, mPos.y);
	m_HoveredObject = m_ActiveScene->GetObjectWithID(pData);

	m_SceneFramebuffer.Unbind();


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
	m_ProjectPanel.ImGuiRender();

	GameWindow();
	SceneWindow();

	DockSpaceEnd();
}

void Editor::GameWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Game");

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 start = ImGui::GetCursorScreenPos();
	ImVec2 end = ImVec2{ start.x + size.x, start.y + size.y };
	drawList->AddImage((void*)m_GameFramebuffer.GetColorAttachmentID(), start, end, { 0, 1 }, { 1, 0 });
	
	m_ActiveScene->Resize(size.x, size.y);

	ImVec2 cursorPos = ImGui::GetCursorPos();
	glViewport(0, 0, size.x, size.y);

	ImGui::End();
	ImGui::PopStyleVar();
}

void Editor::SceneWindow()
{
	ImGuizmo::BeginFrame();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene");


	m_SceneWindowSize = ImGui::GetWindowSize();

	glViewport(0, 0, m_SceneWindowSize.x, m_SceneWindowSize.y);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 size = m_SceneWindowSize;
	ImVec2 start = ImGui::GetWindowPos();
	ImVec2 end = ImVec2{ start.x + size.x, start.y + size.y };
	drawList->AddImage((void*)m_SceneFramebuffer.GetColorAttachmentID(), start, end, { 0, 1 }, { 1, 0 });

	/*
	//Guizmos
	glm::mat4 matrix = glm::mat4(1.0f);
	Vector2f pos = m_EditorCamera.GetPosition();
	float cameraScale = m_EditorCamera.GetScale();

	Projection = glm::perspective(45.0f, m_SceneWindowSize.x / m_SceneWindowSize.y, 0.1f, 1000.0f);
	glm::mat4 CameraView = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	CameraView = glm::scale(CameraView, glm::vec3(cameraScale, cameraScale, 1.0));
	CameraView = glm::translate(CameraView, glm::vec3(-pos.x, -pos.y * m_EditorCamera.GetRatio(), 0.0f));

//	Projection = glm::ortho(float(-m_SceneWindowSize.x) / 2.0f, float(m_SceneWindowSize.x) / 2.0f,
//		float(-m_SceneWindowSize.y) / 2.0f, float(m_SceneWindowSize.y) / 2.0f, 0.1f, 1000.0f);

	ImGuizmo::SetOrthographic(true);
	//ImGuizmo::DrawGrid((float*)&CameraView, (float*)&Projection, (float*)&matrix, 10.25);

	//ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
	ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();
	ImVec2 offset = ImGui::GetWindowPos();
	ImGuizmo::SetRect(minRegion.x + offset.x, minRegion.y + offset.y,
		m_SceneWindowSize.x, m_SceneWindowSize.y);

	if (m_SelectedObject != nullptr)
	{
		Transform* transform = m_SelectedObject->GetComponent<Transform>();
		glm::vec3 position = glm::vec3(0.0f);
		position.x = transform->position.x * m_EditorCamera.GetRatio();
		position.y = transform->position.y * m_EditorCamera.GetRatio();

		objectTransform = glm::translate(glm::mat4(1.0f), position);
		objectTransform = glm::rotate(objectTransform, -ToRads(transform->angle), glm::vec3(0.0, 0.0, 1.0f));

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(Projection),
			(ImGuizmo::TRANSLATE_X), ImGuizmo::WORLD, glm::value_ptr(objectTransform), nullptr, nullptr);

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(Projection),
			(ImGuizmo::TRANSLATE_Y), ImGuizmo::WORLD, glm::value_ptr(objectTransform), nullptr, nullptr);
		
		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(Projection),
			(ImGuizmo::ROTATE_Z), ImGuizmo::WORLD, glm::value_ptr(objectTransform), nullptr, nullptr);

		transform->position.x = objectTransform[3][0] * (1 / m_EditorCamera.GetRatio());
		transform->position.y = objectTransform[3][1] * (1 / m_EditorCamera.GetRatio());
		transform->angle = -ToDegrees(acos(objectTransform[0][0]));
	}

	if (!ImGuizmo::IsUsing())
	{
		if (ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(Mouse::Button0))
			m_SelectedObject = m_HoveredObject;
	}
	*/
	float aspectRatio = float(m_SceneWindowSize.y) / float(m_SceneWindowSize.x);
	m_EditorCamera.SetRatio(aspectRatio);

	glViewport(0, 0, m_SceneWindowSize.x, m_SceneWindowSize.y);
	m_EditorCamera.Update();

	ImGui::End();
	ImGui::PopStyleVar();
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
