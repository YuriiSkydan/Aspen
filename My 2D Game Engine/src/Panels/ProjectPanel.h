#pragma once
#include "../Renderer/Texture.h"
#include <filesystem>

class ProjectPanel
{
private:
	std::filesystem::path m_CurrentDirectory;
	Texture m_BackArrowIcon;
	Texture m_FolderIcon;
	Texture m_FileIcon;

public:
	ProjectPanel();

	void ImGuiRender();
};

