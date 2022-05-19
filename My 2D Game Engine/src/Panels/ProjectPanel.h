#pragma once
#include <filesystem>

class ProjectPanel
{
private:
	std::filesystem::path m_CurrentDirectory;

public:
	ProjectPanel();

	void ImGuiRender();
};

