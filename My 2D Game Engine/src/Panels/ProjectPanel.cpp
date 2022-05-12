#include "ProjectPanel.h"
#include "imgui.h"

ProjectPanel::ProjectPanel()
{
}

void ProjectPanel::ImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Project ");

	for (auto& directoryEntity : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntity.path();
		//auto relativePath = std::filesystem::relative(path, g_AssetPath);
	}

	ImGui::End();
	ImGui::PopStyleVar();
}
