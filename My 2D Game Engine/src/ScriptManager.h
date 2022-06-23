#pragma once
#include <filesystem>
#include <unordered_map>
#include <string_view>
#include "windows.h"
#include "Log/Log.h"
#include "Scene/Scene.h"
#include "Components/Script.h"

#include <iostream>

class ScriptManager
{
private:
	typedef Script* (__stdcall* ScriptCreatePtr)();
	typedef void(__stdcall* ScriptDestroyPtr)(Script*);

	class ScriptDLL
	{
	private:
		std::string m_Path;

		ScriptCreatePtr m_CreateFunction;
		ScriptDestroyPtr m_DestroyFunction;

		HINSTANCE m_DLL;

	public:
		ScriptDLL(HINSTANCE dll)
			: m_DLL(dll)
		{
			m_CreateFunction = ScriptCreatePtr(GetProcAddress(m_DLL, "Create"));
			m_DestroyFunction = ScriptDestroyPtr(GetProcAddress(m_DLL, "Destroy"));
		}

		Script* Create() const
		{
			return m_CreateFunction();
		}
		void Destroy(Script* script)
		{
			m_DestroyFunction(script);
		}

		~ScriptDLL()
		{
			std::cout << "Free DLL!!!\n";
			FreeLibrary(m_DLL);
		}
	};
	
	std::shared_ptr<Scene>& m_Scene;
	//std::unordered_map<std::string, std::pair<std::filesystem::path, std::unique_ptr<ScriptDLL>>> m_Scripts;
	std::unordered_map<std::string, std::unique_ptr<ScriptDLL>> m_Scripts;
	std::filesystem::file_time_type m_LastChangeTime;

	inline static ScriptManager* m_Instance;

private:
	void FindScriptsInDirectory(const std::filesystem::path& directory);

public:
	ScriptManager(std::shared_ptr<Scene>& scene);
	void Update();

	const auto& GetScripts() { return m_Scripts; }

	static void Init(std::shared_ptr<Scene>& scene);
	static ScriptManager& GetInstance();
};

