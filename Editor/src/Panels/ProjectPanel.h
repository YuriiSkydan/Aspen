#pragma once
#include "src/Renderer/Texture.h"
#include <filesystem>
#include <map>
#include <variant>
#include <cassert>

class ProjectPanel
{
private:
	std::filesystem::path m_CurrentDirectory;

	std::map<std::filesystem::directory_entry, std::variant<std::shared_ptr<Texture>, Texture*>> m_DirectoryFiles;

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
};

