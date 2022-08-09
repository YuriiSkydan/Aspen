#include <algorithm>

#include "ProjectPanel.h"
#include "imgui/imgui.h"
using namespace std::string_literals;

ProjectPanel::ProjectPanel()
	: m_CurrentDirectory("Assets")
	, m_BackArrowIcon("Resources/BackArrow_2.png")
	, m_FolderIcon("Resources/FolderIcon_3.png")
	, m_FileIcon("Resources/FileIcon_2.png")
	, m_CppFileIcon("Resources/CppFileIcon_2.png")
{
}

void ProjectPanel::ImGuiRender()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Project ");

	if (ImGui::IsWindowFocused())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, {});
		ImGui::ImageButton((ImTextureID)m_BackArrowIcon.GetID(), { 30, 30 }, { 0, 1 }, { 1, 0 });
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

		std::vector<std::filesystem::directory_entry> files;
		for (auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			files.push_back(directory);
		}

		std::sort(files.begin(), files.end(),
			[](std::filesystem::directory_entry& a, std::filesystem::directory_entry& b)
			{
				if (a.is_directory())
				{
					if (b.is_directory())
						return a < b;
					else
						return true;
				}
				else if (b.is_directory())
					return false;

				return a > b;
			});

		m_FilesIcons.clear();
		for (auto& file : files)
		{
			const auto& path = file.path();
			auto relativePath = std::filesystem::relative(path, "Assets");
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());

			Texture* icon = &m_FolderIcon;

			if (!file.is_directory())
			{
				if (file.path().extension() == ".cpp"s)
				{
					icon = &m_CppFileIcon;
				}
				else if (file.path().extension() == ".png" ||
					file.path().extension() == ".jpg")
				{
					std::wstring wPath = file.path();
					std::string path(wPath.begin(), wPath.end());
					//m_FilesIcons.push_back(TextureLibrary::Get()->GetTexture(path));
					icon = m_FilesIcons.back().get();
				}
				else
				{
					icon = &m_FileIcon;
				}
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
				if (file.is_directory())
					m_CurrentDirectory /= path.filename();
				//else if(file.path().extension() == ".scene")
				//m
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		ImGui::Columns(1);
	}
	else
	{
		float padding = 15;
		float iconSize = 90;
		float cellSize = iconSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		for (auto& icon : m_FilesIcons)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::ImageButton((ImTextureID)icon->GetID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();
			ImGui::NextColumn();

		}
		ImGui::Columns(1);
	}
	ImGui::End();
}
