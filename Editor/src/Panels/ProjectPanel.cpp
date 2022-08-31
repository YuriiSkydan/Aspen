#include <algorithm>
#include "ProjectPanel.h"
#include "Scene/SceneManager.h"
#include "imgui/imgui.h"
using namespace std::string_literals;

ProjectPanel::ProjectPanel()
	: m_CurrentDirectory("Assets")
	, m_BackArrowIcon("Resources/BackArrow_2.png")
	, m_FolderIcon("Resources/FolderIcon_3.png")
	, m_FileIcon("Resources/FileIcon_2.png")
	, m_CppFileIcon("Resources/CppFileIcon_2.png")
	, m_SceneFileIcon("Resources/SceneIcon.png")
	, m_AudioFileIcon("Resources/AudioIcon.png")
{ }

void ProjectPanel::ImGuiRender()
{
	ImGui::Begin("Project ");

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::PushStyleColor(ImGuiCol_Button, {});
	ImGui::ImageButton((ImTextureID)m_BackArrowIcon.GetID(), { 30, 30 }, { 0, 1 }, { 1, 0 });
	ImGui::PopStyleColor();

	if (ImGui::IsItemClicked() && m_CurrentDirectory != "Assets")
	{
		m_CurrentDirectory = m_CurrentDirectory.parent_path();
		m_IsNewDirectory = true;
	}

	if (m_IsNewDirectory)
	{
		UpdateDirectoryData();
	}

	RenderDirectory();

	ImGui::End();
}

void ProjectPanel::UpdateDirectoryData()
{
	m_IsNewDirectory = false;
	m_DirectoryFiles.clear();

	for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		SetEntryIcon(entry);
	}

	std::sort(m_DirectoryFiles.begin(), m_DirectoryFiles.end(),
		[](auto& entry1, auto& entry2)
		{
			if (entry1.first.is_directory())
			{
				if (entry2.first.is_directory())
					return false;

				return true;
			}
			if (entry2.first.is_directory())
				return false;

			return entry1.first > entry2.first;
		});
}

void ProjectPanel::RenderDirectory()
{
	float padding = 15;
	float iconSize = 90;
	float cellSize = iconSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);
	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	for (auto& file : m_DirectoryFiles)
	{
		const auto& path = file.first.path();
		std::string filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str());

		Texture* icon = GetIconPtr(file.second);

		ImGui::ImageButton((ImTextureID)icon->GetID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropSource())
		{
			const std::string itemPath = path.string().c_str();
			ImGui::SetDragDropPayload("PROJECT_PANEL_ITEM", itemPath.c_str(), (itemPath.size() + 1) * sizeof(char));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (file.first.is_directory())
			{
				m_CurrentDirectory /= path.filename();
				m_IsNewDirectory = true;
			}
			else if (file.first.path().extension() == ".scene")
			{
				SceneManager::LoadScene(file.first.path().string());
			}
		}

		ImGui::TextWrapped(filenameString.c_str());
		ImGui::NextColumn();
		ImGui::PopID();
	}

	ImGui::PopStyleColor();
	ImGui::Columns(1);
}

void ProjectPanel::SetEntryIcon(const std::filesystem::directory_entry& entry)
{
	if (!entry.is_directory())
	{
		if (entry.path().extension() == ".png" ||
			entry.path().extension() == ".jpg")
		{
			std::shared_ptr<Texture> image;
			TextureLibrary::Get()->GetTexture(entry.path().string(), image);
			m_DirectoryFiles.push_back({ entry, image });
		}
		else if (entry.path().extension() == ".cpp"s)
		{
			m_DirectoryFiles.push_back({ entry, &m_CppFileIcon });
		}
		else if (entry.path().extension() == ".scene")
		{
			m_DirectoryFiles.push_back({ entry, &m_SceneFileIcon });
		}
		else if (entry.path().extension() == ".mp3")
		{
			m_DirectoryFiles.push_back({ entry, &m_AudioFileIcon });
		}
		else
		{
			m_DirectoryFiles.push_back({ entry, &m_FileIcon });
		}
	}
	else
	{
		m_DirectoryFiles.push_back({ entry, &m_FolderIcon });
	}
}

Texture* ProjectPanel::GetIconPtr(std::variant<std::shared_ptr<Texture>, Texture*>& icon)
{
	if (std::get_if<Texture*>(&icon) == nullptr)
		return std::get<std::shared_ptr<Texture>>(icon).get();

	return std::get<Texture*>(icon);
}