#include "ProjectPanel.h"
#include "imgui/imgui.h"
using namespace std::string_literals;

ProjectPanel::ProjectPanel()
	: m_CurrentDirectory("Assets")
	, m_BackArrowIcon("Resources/BackArrow.png")
	, m_FolderIcon("Resources/FolderIcon_2.png")
	, m_FileIcon("Resources/FileIcon_2.png")
	, m_CppFileIcon("Resources/CppFileIcon_2.png")
{
}

void ProjectPanel::ImGuiRender()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Project ");

	ImGui::PushStyleColor(ImGuiCol_Button, {});
	ImGui::ImageButton((ImTextureID)m_BackArrowIcon.GetID(), { 30, 30 });
	ImGui::PopStyleColor();

	if (ImGui::IsItemClicked() && m_CurrentDirectory != "Assets")
	{
		m_CurrentDirectory = m_CurrentDirectory.parent_path();
	}

	float padding = 15;
	float iconSize = 90;
	float cellSize = iconSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directory.path();
		auto relativePath = std::filesystem::relative(path, "Assets");
		std::string filenameString = relativePath.filename().string();

		ImGui::PushID(filenameString.c_str());

		Texture* icon = &m_FolderIcon;

		if (!directory.is_directory())
		{
			if (directory.path().extension() == ".cpp"s)
				icon = &m_CppFileIcon;
			else
				icon = &m_FileIcon;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		ImGui::ImageButton((ImTextureID)icon->GetID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
		ImGui::PopStyleColor();

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("PROJECT_PANEL_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directory.is_directory())
				m_CurrentDirectory /= path.filename();
		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}
	ImGui::Columns(1);

	ImGui::End();
}
