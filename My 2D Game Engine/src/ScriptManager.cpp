#include "ScriptManager.h"
using namespace std::string_literals;

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
			std::string filePathDLL(filePath.substr(0, filePath.find_last_of('.')));
			filePathDLL += ".dll";

			system(("g++ -shared -o "s + filePathDLL + " " + filePath).c_str());
			
			std::pair<std::filesystem::path, std::unique_ptr<DLL>> newElement;
			newElement = std::make_pair(filePath, std::make_unique<DLL>(filePathDLL));
			m_Scripts.insert(std::move(newElement));
			
			//HINSTANCE hinstance = LoadLibraryA(filePathDLL.c_str());
			
			//if (!hinstance)
			//	std::cout << "Failed to open dll!!!\n";
		
			/*auto func = GetProcAddress(m_Scripts[filePath]->m_DLL, "Test");
			func();*/
		}	
	}
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
	static ScriptManager scriptManager;
	return scriptManager;
}
