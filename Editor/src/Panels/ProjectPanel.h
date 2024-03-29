#pragma once
#include "src/Renderer/Texture.h"
#include <filesystem>
#include <variant>
#include <cassert>

class ProjectPanel
{
private:
	std::filesystem::path m_CurrentDirectory;

	std::vector<std::pair<std::filesystem::directory_entry, std::variant<std::shared_ptr<Texture>, Texture*>>> m_DirectoryFiles;

	bool m_IsNewDirectory = true;

	Texture m_BackArrowIcon;
	Texture m_FolderIcon;
	Texture m_FileIcon;
	Texture m_CppFileIcon;
	Texture m_SceneFileIcon;
	Texture m_AudioFileIcon;

public:
	ProjectPanel();

	void ImGuiRender();
	void UpdateDirectoryData();
	void RenderDirectory();
	void SetEntryIcon(const std::filesystem::directory_entry& entry);
	Texture* GetIconPtr(std::variant<std::shared_ptr<Texture>, Texture*>& icon);
};

