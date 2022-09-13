#pragma once
#include <filesystem>
#include <unordered_map>
#include <string_view>
#include "windows.h"
#include "../Log/Log.h"
#include "../Scene/Scene.h"
#include "../Components/Script.h"
#include "../Engine/Engine.h"

using namespace std::string_literals;

class ASPEN ScriptManager
{
private:
	typedef Script* (__stdcall* ScriptCreatePtr)();

	class ScriptDLL
	{
	private:
		std::filesystem::file_time_type m_AddedTime;
		std::string m_ScriptName;
		ScriptCreatePtr m_CreateFunction;
		HINSTANCE m_DLL;

	public:
		ScriptDLL(HINSTANCE dll, const std::string& scriptName, std::filesystem::file_time_type addedTime)
			: m_DLL(dll), m_ScriptName(scriptName), m_AddedTime(addedTime)
		{
			m_CreateFunction = ScriptCreatePtr(GetProcAddress(m_DLL, ("Create" + scriptName).c_str()));
		
			if (m_CreateFunction == nullptr)
			{
				WARN("Failed to load create function!!!\n");
			}
		}
		Script* Create() const
		{
			return m_CreateFunction();
		}
		std::filesystem::file_time_type GetAddedTime() const { return m_AddedTime; }
		~ScriptDLL()
		{
			INFO("Free DLL");
			FreeLibrary(m_DLL);
		}
	};
	
	//std::unordered_map<std::string, std::pair<std::filesystem::path, std::unique_ptr<ScriptDLL>>> m_Scripts;
	std::unordered_map<std::string, std::unique_ptr<ScriptDLL>> m_Scripts;
	std::filesystem::file_time_type m_LastChangeTime;

	inline static ScriptManager* m_Instance;

private:
	void FindScriptsInDirectory(const std::filesystem::path& directory);
	bool FindDLL(const std::string& filename, std::filesystem::directory_entry& dllEntry);
	bool LoadDLL(const std::filesystem::directory_entry& entry);
	void CompileCpp(const std::filesystem::path& filepath);
	std::string GetFilename(const std::filesystem::path& path);

public:
	ScriptManager() = default;
	ScriptManager(const ScriptManager& other) = delete;
	const ScriptManager& operator=(const ScriptManager& other) = delete;

	void Update();

	const auto& GetScripts() { return m_Scripts; }

	static ScriptManager& Get();
};