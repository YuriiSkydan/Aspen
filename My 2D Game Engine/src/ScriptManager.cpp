#include "ScriptManager.h"
using namespace std::string_literals;

#include "Components/Script.h"

ScriptManager::ScriptManager(std::shared_ptr<Scene>& scene)
	: m_Scene(scene)
{
}

void ScriptManager::FindScriptsInDirectory(const std::filesystem::path& directory)
{
	for (auto& it : std::filesystem::directory_iterator(directory))
	{
		if (it.is_directory())
			FindScriptsInDirectory(it.path());

		if (it.path().extension() == ".cpp")
		{
			std::wstring path = it.path();
			std::string filePath(path.begin(), path.end());

			size_t lastDot = filePath.find_last_of('.');
			std::string filePathDLL(filePath.substr(0, lastDot) + ".dll");
		
			size_t end = filePath.find_last_of('\\') + 1;
			std::string filename(filePath.substr(end, lastDot - end));

			if (m_Scripts.find(filename) != m_Scripts.end())
			{
				std::cout << "Script is already created!!!\n";
				m_Scripts.erase(filename);
			}

			//system(("g++ -shared -MD -MP -MF -o "s + filePathDLL + " " + filePath + " -D DLL_BUILD").c_str());

			//system("g++ -c src/Components/Component.cpp src/Components/Component.h");
			//system(("g++ -std=c++17 -shared -o "s + filePathDLL + " " + filePath + " -D DLL_BUILD").c_str());
			system(("call \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" && cl /EHsc /MDd /LD "s + filePath).c_str());
			system("move \"Player.dll\" \"\Assets\"");
			system("move \"Player.lib\" \"\Assets\"");
			system("move \"Player.obj\" \"\Assets\"");
			system("move \"Player.exp\" \"\Assets\"");
			//system(("cl /EHsc /LD "s + filePath).c_str());

			HINSTANCE dll;
			dll = LoadLibraryA(filePathDLL.c_str());

			if (dll)
			{
				//std::pair<std::filesystem::path, std::unique_ptr<ScriptDLL>> newElement;
				//newElement = std::make_pair(filePath, std::make_unique<ScriptDLL>(dll));
				//m_Scripts.insert(std::move(newElement));

				std::pair<std::string, std::unique_ptr<ScriptDLL>> newElement;
				newElement = std::make_pair(filename, std::make_unique<ScriptDLL>(dll));
				m_Scripts.insert(std::move(newElement));
			}
			else
			{
				ERROR("Failed to load DLL!!!\n");
			}
		}
	}
}

void ScriptManager::Init(std::shared_ptr<Scene>& scene)
{
	static ScriptManager scriptManager(scene);
	m_Instance = &scriptManager;
}

void ScriptManager::Update()
{
	if (std::filesystem::last_write_time("Assets") > m_LastChangeTime)
	{
		//system("g++ -shared -o Assets\\Test.dll Assets\\Test.cpp");

		FindScriptsInDirectory("Assets");
		m_LastChangeTime = std::filesystem::last_write_time("Assets");
	}
}

ScriptManager& ScriptManager::GetInstance()
{
	return *m_Instance;
}
