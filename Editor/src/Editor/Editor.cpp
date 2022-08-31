#include "Editor.h"
#include "../ImGuizmo/ImSequencer.h"

#include "src/Math/Math.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

#include "src/Input/Input.h"
#include "src/Renderer/Renderer.h"
#include "src/Engine/Engine.h"
#include "src/ScriptManager.h"
#include "Components/Transform.h"
#include "Components/BoxCollider.h"
#include "Components/CircleCollider.h"
#include "Components/PolygonCollider.h"

#include "../Utils/File.h"

Editor::Editor()
	: Layer("Editor")
	, m_HierarchyPanel(m_SelectedObject)
	, m_InspectorPanel(m_SelectedObject)
	, m_EditorCamera(1080.0f / 1920.0f)
	, m_PlayButtonIcon("Resources/PlayIcon.png")
	, m_StopButtonIcon("Resources/StopIcon.png")
	, m_PauseButtonIcon("Resources/PauseIcon.png")
	, m_MoveButtonIcon("Resources/MoveIcon.png")
	, m_RotateButtonIcon("Resources/RotateIcon.png")
	, m_ScaleButtonIcon("Resources/ScaleIcon.png")
	, m_ToolbarHeight(33)
{
	ImGui::SetCurrentContext(Engine::Get().GetImGuiContext());

	m_EditorScene = std::make_shared<Scene>();
	SceneManager::SetActiveScene(m_EditorScene);
	//m_ActiveScene = m_EditorScene;

	//m_HierarchyPanel.SetScene(m_ActiveScene);

	//Setting up frame buffers
	m_SceneFramebuffer.Bind();
	m_SceneFramebuffer.AddColorAttachment(GL_RGBA8, GL_RGBA);
	m_SceneFramebuffer.AddColorAttachment(GL_R32I, GL_RED_INTEGER);
	m_SceneFramebuffer.DrawBuffers();

	m_GameFramebuffer.Bind();
	m_GameFramebuffer.AddColorAttachment(GL_RGBA8, GL_RGBA);

	//Initial guizmo operation
	m_CurrentOperation = ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y;

	DarkStyle();
}

void Editor::Update()
{

#pragma region RenderScene
	if (m_SceneWindowSize.x != m_SceneFramebuffer.GetWidth() ||
		m_SceneWindowSize.y != m_SceneFramebuffer.GetHeight())
	{
		float aspectRatio = float(m_SceneWindowSize.x) / float(m_SceneWindowSize.y);

		m_SceneFramebuffer.Resize(m_SceneWindowSize.x, m_SceneWindowSize.y);
		m_EditorCamera.SetRatio(aspectRatio);
		m_SceneFramebuffer.Unbind();
	}

	m_SceneFramebuffer.Bind();

	SceneManager::GetActiveScene()->UpdateOnEditor(m_EditorCamera);

	//Render Colliders
	if (m_SelectedObject != nullptr && m_SelectedObject->IsActive())
	{
		BoxCollider* boxCollider = m_SelectedObject->GetComponent<BoxCollider>();
		if (boxCollider != nullptr && boxCollider->IsEnabled())
		{
			Renderer::DrawBoxCollider(boxCollider, m_EditorCamera.GetCameraMatrix());
		}

		CircleCollider* circleCollider = m_SelectedObject->GetComponent<CircleCollider>();
		if (circleCollider != nullptr && circleCollider->IsEnabled())
		{
			Renderer::DrawCirlceCollider(circleCollider, m_EditorCamera.GetCameraMatrix());
		}

		PolygonCollider* polygonCollider = m_SelectedObject->GetComponent<PolygonCollider>();
		if (polygonCollider != nullptr && polygonCollider->IsEnabled())
		{
			Renderer::DrawPolygonCollider(polygonCollider, m_EditorCamera.GetCameraMatrix());
		}
	}

	m_SceneFramebuffer.Unbind();
#pragma endregion

#pragma region RenderGame
	if (m_GameWindowSize.x != m_GameFramebuffer.GetWidth() ||
		m_GameWindowSize.y != m_GameFramebuffer.GetHeight() ||
		m_GameWindowSize.x != SceneManager::GetActiveScene()->GetWidth() ||
		m_GameWindowSize.y != SceneManager::GetActiveScene()->GetHeight())
	{
		m_GameFramebuffer.Resize(m_GameWindowSize.x, m_GameWindowSize.y);
		SceneManager::GetActiveScene()->Resize(m_GameWindowSize.x, m_GameWindowSize.y);
	}

	m_GameFramebuffer.Bind();
	SceneManager::GetActiveScene()->Render();
	m_GameFramebuffer.Unbind();
#pragma endregion

	switch (m_SceneState)
	{
	case SceneState::PLAY:
		SceneManager::GetActiveScene()->Update();
		//m_ActiveScene->Update();
		break;
	case SceneState::EDIT:
		ScriptManager::Get().Update();
		break;
	}
}

void Editor::ImGuiRender()
{
	MainMenuBar();
	DockSpace();
	Toolbar();

	m_HierarchyPanel.ImGuiRender();
	m_InspectorPanel.ImGuiRender();
	m_ProjectPanel.ImGuiRender();

	GameWindow();
	SceneWindow();

	//ImGui::ShowDemoWindow();
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

	if (m_GameWindowSize.x != size.x || m_GameWindowSize.y != size.y)
		m_GameWindowSize = size;

	ImGui::End();
	ImGui::PopStyleVar();
}

void Editor::ProjectPreferencesWindow()
{
	ImGui::Begin("Project Preferences");
}

void Editor::SceneWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar);

	ImGui::Dummy({ m_SceneWindowSize.x, m_SceneWindowSize.y - 30 });
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROJECT_PANEL_ITEM"))
		{
			const std::string path = (const char*)payload->Data;

			if (File::IsOfTypes(path, { ".png", ".jpg", ".jpeg" }))
			{
				size_t startPos = path.find_last_of("\\") + 1;
				size_t endPos = path.find_last_of(".") - startPos;

				std::string objectName = path.substr(startPos, endPos);

				auto activeScene = SceneManager::GetActiveScene();
				GameObject* gameObject = activeScene->CreateGameObject(objectName);
				SpriteRenderer* spriteRenderer = gameObject->AddComponent<SpriteRenderer>();
				spriteRenderer->SetSprite(path);

				Vector2f objectPosition;
				Matrix3x3f cameraMatrix = m_EditorCamera.GetCameraMatrix();
				objectPosition.x = -((1.0f / cameraMatrix[0][0]) * cameraMatrix[0][2]);
				objectPosition.y = -((1.0f / cameraMatrix[1][1]) * cameraMatrix[1][2]);

				ImVec2 mousePosition = ImGui::GetMousePos();
				auto offset = ImGui::GetWindowPos();
				ImVec2 windowCenter;
				windowCenter.x = offset.x + m_SceneWindowSize.x / 2.0f;
				windowCenter.y = offset.y + m_SceneWindowSize.y / 2.0f;

				objectPosition.x += ((mousePosition.x - windowCenter.x) / (m_SceneWindowSize.x / 2.0f)) * (1.0f / cameraMatrix[0][0]);
				objectPosition.y -= ((mousePosition.y - windowCenter.y) / (m_SceneWindowSize.y / 2.0f)) * (1.0f / cameraMatrix[1][1]);

				gameObject->transform->position = objectPosition;

				m_SelectedObject = gameObject;
			}
			else if (File::IsOfType(path, ".scene"))
			{
				OpenScene(path);
			}

			ImGui::EndDragDropTarget();
		}
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 start = ImGui::GetWindowPos();
	ImVec2 end = ImVec2{ start.x + size.x, start.y + size.y };

	if (m_SceneWindowSize.x != size.x || m_SceneWindowSize.y != size.y)
		m_SceneWindowSize = size;

	//Setting Scene Framebuffer
	drawList->AddImage((void*)m_SceneFramebuffer.GetColorAttachmentID(), start, end, { 0, 1 }, { 1, 0 });

#pragma region GuizmoToolBar
	ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 0.2 });
	ImVec2 buttonSize{ 20, 20 };
	ImGui::SameLine(m_SceneWindowSize.x - 100);
	if (ImGui::ImageButton(ImTextureID(m_MoveButtonIcon.GetID()), buttonSize))
	{
		m_CurrentOperation = ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y;
	}

	ImGui::SameLine(m_SceneWindowSize.x - 70);
	if (ImGui::ImageButton(ImTextureID(m_RotateButtonIcon.GetID()), buttonSize))
	{
		m_CurrentOperation = ImGuizmo::ROTATE_Z;
	}

	ImGui::SameLine(m_SceneWindowSize.x - 40);
	if (ImGui::ImageButton(ImTextureID(m_ScaleButtonIcon.GetID()), buttonSize))
	{
		m_CurrentOperation = ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y;
	}
	ImGui::PopStyleColor();
#pragma endregion

	UpdateGuizmo();

	if (!ImGuizmo::IsUsing() && !ImGui::IsItemClicked())
	{
		if (ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(Mouse::Button0))
		{
			//Pick up object
			auto mousePos = ImGui::GetMousePos();
			auto offset = ImGui::GetWindowPos();

			mousePos.x -= offset.x;
			mousePos.y = m_SceneWindowSize.y / 2.0f + (m_SceneWindowSize.y / 2.0f - mousePos.y);
			mousePos.y += offset.y;

			m_SceneFramebuffer.Bind();
			int pData = m_SceneFramebuffer.ReadPixel(1, mousePos.x, mousePos.y);
			m_SceneFramebuffer.Unbind();

			m_SelectedObject = SceneManager::GetActiveScene()->GetObjectWithID(pData);
		}
	}

	if (ImGui::IsWindowHovered())
		m_EditorCamera.Update();

	ImGui::End();
	ImGui::PopStyleVar();
}

void Editor::OpenScene()
{
	if (m_SceneState != SceneState::PLAY)
	{
		auto newScene = std::make_shared<Scene>();

		using namespace nlohmann;
		std::ifstream fileStream(newScene->GetName() + ".scene", std::ofstream::binary);

		if (!fileStream.is_open())
		{
			ERROR("Failed to open the file!!!");
			return;
		}

		json in;
		fileStream >> in;

		newScene->Deserialize(in);

		fileStream.close();

		if (newScene != nullptr)
		{
			m_EditorScene = newScene;
			//m_ActiveScene = m_EditorScene;
			SceneManager::SetActiveScene(m_EditorScene);
			m_SelectedObject = nullptr;

			//m_HierarchyPanel.SetScene(m_ActiveScene);
		}
	}
	else
	{
		WARN("Exit play mode to open scene.");
	}
}

void Editor::OpenScene(std::string_view path)
{
	if (m_SceneState != SceneState::PLAY)
	{
		auto newScene = std::make_shared<Scene>();

		newScene->Deserialize(path);

		if (newScene != nullptr)
		{
			m_EditorScene = newScene;
			SceneManager::SetActiveScene(m_EditorScene);
			//m_ActiveScene = m_EditorScene;
			m_SelectedObject = nullptr;

			//m_HierarchyPanel.SetScene(m_ActiveScene);
		}
	}
	else
	{
		WARN("Exit play mode to open scene.");
	}
}

void Editor::SaveScene()
{
	if (m_SceneState != SceneState::PLAY)
	{
		SceneManager::GetActiveScene()->Serialize();
	}
	else
	{
		WARN("Exit play mode to save scene.");
	}
}

void Editor::SaveSceneAs()
{

}

void Editor::UpdateGuizmo()
{
	Vector2f pos = m_EditorCamera.GetPosition();
	float cameraScale = m_EditorCamera.GetScale();

	ImGuizmo::SetOrthographic(true);

	{
		auto rect = m_EditorCamera.GetCameraMatrix();
		float XAxis = (1.0f / rect[0][0]);
		float YAxis = (1.0f / rect[1][1]);

		glm::mat4 Projection = glm::ortho(-XAxis, XAxis, -YAxis, YAxis);

		Vector2f cameraPos = m_EditorCamera.GetPosition();
		glm::mat4 CameraView = glm::lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		CameraView = glm::translate(CameraView, glm::vec3(cameraPos.x * XAxis, 0.0f, -cameraPos.y * YAxis));

		glm::mat4 matrix(10.0f);

		static float grid = 0.0f;
		if (Input::IsKeyPressed(Key::Right))
			grid += 0.01f;
		if (Input::IsKeyPressed(Key::Left))
			grid -= 0.01f;

		//ImGuizmo::DrawGrid((float*)&CameraView, (float*)&Projection, (float*)&matrix, 100.0f);
	}

	//ImGuizmo::SetOrthographic(false);


	ImGuizmo::SetDrawlist();

	ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
	ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();
	ImVec2 offset = ImGui::GetWindowPos();
	ImGuizmo::SetRect(minRegion.x + offset.x, minRegion.y + offset.y,
		m_SceneWindowSize.x, m_SceneWindowSize.y);

	if (m_SelectedObject != nullptr)
	{
		Transform* transform = m_SelectedObject->transform;
		glm::vec3 position = glm::vec3(0.0f);
		position.x = transform->position.x;
		position.y = transform->position.y;

		glm::vec3 scale = glm::vec3(1.0f);
		scale.x = transform->scale.x;
		scale.y = transform->scale.y;

		auto rect = m_EditorCamera.GetCameraMatrix();
		float XAxis = (1.0f / rect[0][0]);
		float YAxis = (1.0f / rect[1][1]);

		glm::mat4 Projection = glm::ortho(-XAxis, XAxis, -YAxis, YAxis);
		glm::mat4 objectTransform = glm::translate(glm::mat4(1.0f), position);
		objectTransform = glm::scale(objectTransform, scale);
		//objectTransform = glm::rotate(objectTransform, -ToRads(transform->angle), glm::vec3(0.0, 0.0, 1.0f));

		Matrix3x3f cameraMatrix = m_EditorCamera.GetCameraMatrix();
		Vector2f cameraPos = m_EditorCamera.GetPosition();
		cameraPos.x *= cameraMatrix[0][0];
		cameraPos.y *= cameraMatrix[1][1];

		glm::mat4 CameraView = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		CameraView = glm::translate(CameraView, glm::vec3(-cameraPos.x * XAxis, -cameraPos.y * YAxis, 0.0f));

		ImGuizmo::SetLineThickness(6.3f);

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(Projection),
			(m_CurrentOperation), ImGuizmo::WORLD, glm::value_ptr(objectTransform));

		transform->position.x = objectTransform[3][0];
		transform->position.y = objectTransform[3][1];

		transform->scale.x = Vector2f(objectTransform[0][0], objectTransform[1][0]).Magnitude();
		transform->scale.y = Vector2f(objectTransform[1][0], objectTransform[1][1]).Magnitude();

		transform->angle -= ToDegrees(asin(objectTransform[0][1]));
	}
}

void Editor::Toolbar()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 19));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_ToolbarHeight));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin("TOOLBAR", NULL, window_flags);
	ImGui::PopStyleVar();

	int xPos = ImGui::GetWindowSize().x / 2;
	ImGui::SameLine(xPos - 36);

	if (m_SceneState == SceneState::EDIT)
	{
		if (ImGui::ImageButton((ImTextureID)m_PlayButtonIcon.GetID(), ImVec2(18, 18)))
		{
			m_SceneState = SceneState::PLAY;

			auto m_RuntimeScene = std::make_shared<Scene>();
			m_RuntimeScene->Copy(*m_EditorScene);

			//m_ActiveScene = m_RuntimeScene;
			SceneManager::SetActiveScene(m_RuntimeScene);
			SceneManager::GetActiveScene()->Start();
			
			m_SelectedObject = nullptr;
		}
	}
	else
	{
		if (ImGui::ImageButton((ImTextureID)m_StopButtonIcon.GetID(), ImVec2(18, 18)))
		{
			m_SceneState = SceneState::EDIT;

			SceneManager::GetActiveScene()->Stop();
			SceneManager::SetActiveScene(m_EditorScene);
			//m_ActiveScene = m_EditorScene;

			m_SelectedObject = nullptr;
		}
	}

	ImGui::SameLine(xPos - 9);

	if (ImGui::ImageButton((ImTextureID)m_PauseButtonIcon.GetID(), ImVec2(18, 18)))
	{
		if (m_SceneState != SceneState::EDIT)
		{
			if (m_SceneState == SceneState::PAUSE)
				m_SceneState = SceneState::PLAY;
			else
				m_SceneState = SceneState::PAUSE;
		}
	}

	ImGui::End();
}

void Editor::DockSpace()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos({ viewport->WorkPos.x, viewport->Pos.y + m_ToolbarHeight });
	ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y - m_ToolbarHeight });
	ImGui::SetNextWindowViewport(viewport->ID);

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("DockSpace", NULL, windowFlags);

	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceID);
	}

	ImGui::End();
}

void Editor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"));

			if (ImGui::MenuItem("Open Scene"))
				OpenScene();

			ImGui::Separator();

			if (ImGui::MenuItem("Save"))
				SaveScene();

			if (ImGui::MenuItem("Save As..."));
			ImGui::Separator();

			if (ImGui::MenuItem("New Project"));
			if (ImGui::MenuItem("Open Project"));
			if (ImGui::MenuItem("Save Project"));
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Gamebject"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::BeginMenu("Themes"))
			{
				if (ImGui::MenuItem("Black Style"))
					BlackStyle();
				if (ImGui::MenuItem("Corporate Grey Style"))
					CorporateGreyStyle();
				if (ImGui::MenuItem("Dark Style"))
					DarkStyle();
				if (ImGui::MenuItem("Dark Night Style"))
					DarkNightStyle();
				if (ImGui::MenuItem("Light Style"))
					ImGui::StyleColorsLight();

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::BlackStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}

void Editor::CorporateGreyStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	/// 0 = FLAT APPEARENCE
	/// 1 = MORE "3D" LOOK
	int is3D = 0;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding = ImVec2(6, 4);
	style.ItemSpacing = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = is3D;

	style.WindowRounding = 3;
	style.ChildRounding = 3;
	style.FrameRounding = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
	style.TabBorderSize = is3D;
	style.TabRounding = 3;

	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
#endif
}

void Editor::DarkStyle()
{
	//imGuiIO.Fonts->AddFontFromFileTTF("../data/Fonts/Ruda-Bold.ttf", 15.0f, &config);
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Editor::DarkNightStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.GrabRounding = style.FrameRounding = 2.3f;
}