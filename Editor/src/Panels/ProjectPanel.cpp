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

	ImGui::PushStyleColor(ImGuiCol_Button, {});
	ImGui::ImageButton((ImTextureID)m_BackArrowIcon.GetID(), { 30, 30 }, { 0, 1 }, { 1, 0 });
	ImGui::PopStyleColor();

	if (ImGui::IsItemClicked() && m_CurrentDirectory != "Assets")
	{
		m_CurrentDirectory = m_CurrentDirectory.parent_path();
		m_IsNewDirectory = true;
	}

	float padding = 15;
	float iconSize = 90;
	float cellSize = iconSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	if (m_IsNewDirectory)
	{
		m_IsNewDirectory = false;
		m_DirectoryFiles.clear();

		for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (!entry.is_directory())
			{
				if (entry.path().extension() == ".cpp"s)
				{
					m_DirectoryFiles.insert({ entry, &m_CppFileIcon });
				}
				else if (entry.path().extension() == ".png" ||
					entry.path().extension() == ".jpg")
				{
					std::wstring wPath = entry.path();
					std::string path(wPath.begin(), wPath.end());
					std::shared_ptr<Texture> image;
					TextureLibrary::Get()->GetTexture(path, image);
					m_DirectoryFiles.insert({ entry, image });
				}
				else
				{
					m_DirectoryFiles.insert({ entry, &m_FileIcon });
				}
			}
			else
			{
				m_DirectoryFiles.insert({ entry, &m_FolderIcon });
			}
		}
	}

	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	for (auto& file : m_DirectoryFiles)
	{
		const auto& path = file.first.path();
		std::string filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str());

		Texture* icon;
		if (std::get_if<Texture*>(&file.second) == nullptr)
			icon = std::get<std::shared_ptr<Texture>>(file.second).get();
		else
			icon = std::get<Texture*>(file.second);

		ImGui::ImageButton((ImTextureID)icon->GetID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropSource())
		{
			auto relativePath = std::filesystem::relative(path, "Assets");
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("PROJECT_PANEL_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (file.first.is_directory())
			{
				m_CurrentDirectory /= path.filename();
				m_IsNewDirectory = true;
			}
			//else if(file.path().extension() == ".scene")

		}

		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();

	}
	ImGui::PopStyleColor();

	ImGui::Columns(1);

	ImGui::End();
}