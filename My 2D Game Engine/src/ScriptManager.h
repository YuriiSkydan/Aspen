#pragma once
#include <filesystem>
#include <unordered_map>
#include <string_view>
#include "windows.h"
#include "Log/Log.h"

#include <iostream>

typedef void(__stdcall* Function)();

class ScriptManager
{
private:
	class DLL
	{
	private:
		std::string m_Path;

	public:
		HINSTANCE m_DLL;

	public:
		DLL(std::string_view path)
			: m_Path(path), m_DLL(LoadLibraryA(m_Path.c_str()))
		{
			if (!m_DLL)
				ERROR("Failed to load dll!!!\n");
			
		}
		~DLL()
		{
			std::cout << "Free DLL!!!\n";
			FreeLibrary(m_DLL);
		}
	};

	std::unordered_map<std::filesystem::path, std::unique_ptr<DLL>> m_Scripts;
	std::filesystem::file_time_type m_LastChangeTime;

private:
	void FindScriptsInDirectory(const std::filesystem::path& directory);

public:
	void Update();

	const auto& GetScripts() { return m_Scripts; }
	static ScriptManager& GetInstance();
};

