#pragma once
#include <filesystem>
#include <unordered_map>
#include <string_view>
#include "windows.h"
#include "Log/Log.h"
#include "Scene/Scene.h"
#include "Components/Script.h"

#include <iostream>
#include "Engine/Engine.h"

using namespace std::string_literals;

class ASPEN ScriptManager
{
private:
	typedef Script* (__stdcall* ScriptCreatePtr)();

	class ScriptDLL
	{
	private:
		std::string m_ScriptName;
		ScriptCreatePtr m_CreateFunction;
		HINSTANCE m_DLL;

	public:
		ScriptDLL(HINSTANCE dll, const std::string& scriptName)
			: m_DLL(dll), m_ScriptName(scriptName)
		{
			m_CreateFunction = ScriptCreatePtr(GetProcAddress(m_DLL, "Create"));
		}

		Script* Create() const
		{
			return m_CreateFunction();
		}

		~ScriptDLL()
		{
			std::cout << "Free DLL!!!\n";
			FreeLibrary(m_DLL);
		}
	};
	
	//std::unordered_map<std::string, std::pair<std::filesystem::path, std::unique_ptr<ScriptDLL>>> m_Scripts;
	std::unordered_map<std::string, std::unique_ptr<ScriptDLL>> m_Scripts;
	std::filesystem::file_time_type m_LastChangeTime;

	inline static ScriptManager* m_Instance;

private:
	void FindScriptsInDirectory(const std::filesystem::path& directory);

public:
	ScriptManager() = default;

	ScriptManager(const ScriptManager& other) = delete;
	const ScriptManager& operator=(const ScriptManager& other) = delete;

	void Update();

	const auto& GetScripts() { return m_Scripts; }

	static ScriptManager& Get();
};