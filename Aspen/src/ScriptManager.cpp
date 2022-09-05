#include "ScriptManager.h"


void ScriptManager::FindScriptsInDirectory(const std::filesystem::path& directory)
{
	for (auto& it : std::filesystem::directory_iterator(directory))
	{
		if (it.is_directory())
			FindScriptsInDirectory(it.path());

		if (it.path().extension() == ".cpp")
		{
			std::string filePath = it.path().string();

			size_t lastDot = filePath.find_last_of('.');
			std::string filePathDLL(filePath.substr(0, lastDot) + ".dll");

			size_t end = filePath.find_last_of('\\') + 1;
			std::string filename(filePath.substr(end, lastDot - end));

			std::vector<GameObject*> addScriptObjects;
			if (m_Scripts.find(filename) != m_Scripts.end())
			{
				auto scriptObjects = SceneManager::GetActiveScene()->GetObjectsWithComponent<Script>();
				for (auto& object : scriptObjects)
				{
					auto scripts = object->GetScripts();
					for (auto script : scripts)
					{
						if (script->GetName() == filename)
						{
							addScriptObjects.push_back(object);
							script->Remove();
						}
					}
				}

				m_Scripts.erase(filename);
			}

			std::string compileCommand = "call \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" && ";
			compileCommand += " cl /std:c++20";

			//Additional dependecies
			compileCommand += " /I ..\\Aspen";
			compileCommand += " /I ..\\Aspen\\Libraries\\include";
			compileCommand += " /I ..\\Aspen\\Libraries\\include\\glad";
			compileCommand += " /I ..\\Aspen\\Libraries\\include\\GLFW";
			compileCommand += " /I ..\\Aspen\\vendor";
			compileCommand += " /I ..\\Aspen\\vendor\\imgui";
			compileCommand += " /I ..\\Aspen\\vendor\\spdlog-1.x\\include";

			//Compile properties
#ifdef _DEBUG
			compileCommand += " /EHsc /MDd /LD " + filePath;
#endif
#ifdef NDEBUG
			compileCommand += " /EHsc /MD /LD " + filePath;
#endif

			compileCommand += " /link Aspen.lib";

#ifdef _DEBUG
			compileCommand += " /LIBPATH:..\\x64\\Debug";
#endif
#ifdef NDEBUG
			compileCommand += " /LIBPATH:..\\x64\\Release";
#endif

			system(compileCommand.c_str());
			system(("move \""s + filename + ".dll\" \"\Assets\"").c_str());
			system(("move \""s + filename + ".lib\" \"\Assets\"").c_str());
			system(("move \""s + filename + ".obj\" \"\Assets\"").c_str());
			system(("move \""s + filename + ".exp\" \"\Assets\"").c_str());

			HINSTANCE dll;
			dll = LoadLibraryA(filePathDLL.c_str());

			if (dll)
			{
				std::pair<std::string, std::unique_ptr<ScriptDLL>> newElement;
				newElement = std::make_pair(filename, std::make_unique<ScriptDLL>(dll, filename));
				m_Scripts.insert(std::move(newElement));

				for (auto& object : addScriptObjects)
				{
					Script* newScript = m_Scripts[filename]->Create();
					newScript->SetName(filename);
					object->AddScript(newScript);
				}
			}
			else
			{
				ERROR("Failed to load DLL!!!\n");
			}
		}
	}
}

void ScriptManager::Update()
{
	if (std::filesystem::last_write_time("Assets") > m_LastChangeTime)
	{
		FindScriptsInDirectory("Assets");
		m_LastChangeTime = std::filesystem::last_write_time("Assets");
	}
}

ScriptManager& ScriptManager::Get()
{
	static ScriptManager scriptManager;
	m_Instance = &scriptManager;
	return *m_Instance;
}