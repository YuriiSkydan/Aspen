#pragma once
#include "src/Renderer/Texture.h"
#include <filesystem>

class ProjectPanel
{
private:
	std::filesystem::path m_CurrentDirectory;
	std::vector<std::shared_ptr<Texture>> m_FilesIcons;
	Texture m_BackArrowIcon;
	Texture m_FolderIcon;
	Texture m_FileIcon;
	Texture m_CppFileIcon;

public:
	ProjectPanel();

	void ImGuiRender();
};

