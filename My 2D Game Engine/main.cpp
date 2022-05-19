#include "../src/Engine/Engine.h"

int main()
{
	Engine* engine = new Engine();

	engine->Run();

	delete engine;

	return 0;
}


//int main()
//{
//	{
//		if (!glfwInit())
//		{
//			std::cerr << "Failed to initialize GLFW!!!\n";
//			return -1;
//		}
//
//		const char* glslVersion = "#version 130";
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//
//		GLFWwindow* window = glfwCreateWindow(1920, 1080, "My 2D Game Engine", NULL, NULL);
//
//		if (window == nullptr)
//		{
//			std::cerr << "Failed to initialize window!!!\n";
//			glfwTerminate();
//			return -2;
//		}
//
//		glfwMakeContextCurrent(window);
//
//		IMGUI_CHECKVERSION();
//		ImGui::CreateContext();
//		ImGuiIO& io = ImGui::GetIO(); (void)io;
//		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
//
//		ImGui::StyleColorsDark();
//
//		ImGuiStyle& style = ImGui::GetStyle();
//		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//		{
//			style.WindowRounding = 0.0f;
//			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//		}
//
//		ImGui_ImplGlfw_InitForOpenGL(window, true);
//		ImGui_ImplOpenGL3_Init(glslVersion);
//
//		if (!gladLoadGL())
//		{
//			std::cerr << "Failed to load GL!!!";
//			glfwTerminate();
//			return -3;
//		}
//
//		{
//			Scene scene;
//
//			glEnable(GL_DEBUG_OUTPUT);
//			glDebugMessageCallback(DebugMessageCallback, NULL);
//			glfwSetErrorCallback(ErrorCallback);
//
//			//Shader shader("Shaders/StandartShader.vs", "Shaders/StandartShader.fs");
//
//			glm::mat3 transform = glm::mat3(1.0f);
//			glClearColor(0.4f, 0.1f, 0.8f, 1.0);
//			while (!glfwWindowShouldClose(window))
//			{
//				glClear(GL_COLOR_BUFFER_BIT);
//
//				ImGui_ImplOpenGL3_NewFrame();
//				ImGui_ImplGlfw_NewFrame();
//				ImGui::NewFrame();
//				//	ImGui::DockSpaceOverViewport();
//
//				scene.UpdateGameObjects();
//				scene.UpdateHirarchy();
//				scene.UpdateInspector();
//
//				ImGui::Begin("Some window");
//				ImGui::End();
//
//				ImGui::Render();
//				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//				{
//					GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
//					ImGui::UpdatePlatformWindows();
//					ImGui::RenderPlatformWindowsDefault();
//					glfwMakeContextCurrent(backupCurrentContext);
//				}
//
//				glfwSwapBuffers(window);
//				glfwPollEvents();
//			}
//	
//		}
//
//		ImGui_ImplOpenGL3_Shutdown();
//		ImGui_ImplGlfw_Shutdown();
//		ImGui::DestroyContext();
//
//		glfwDestroyWindow(window);
//		glfwTerminate();
//	}
//
//	return 0;
//}