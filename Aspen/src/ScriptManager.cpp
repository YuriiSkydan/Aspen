#include "ScriptManager.h"


void ScriptManager::FindScriptsInDirectory(const std::filesystem::path& directory)
{
	for (auto& it : std::filesystem::directory_iterator(directory))
	{
		if (it.is_directory())
			FindScriptsInDirectory(it.path());

		if (it.path().extension() == ".cpp")
		{
			std::string filename = GetFilename(it.path());
			bool doesExist = m_Scripts.find(filename) != m_Scripts.end();
			bool isOutdated = doesExist ? m_Scripts[filename]->GetAddedTime() < it.last_write_time() : false;

			if (!doesExist)
			{
				std::filesystem::directory_entry dllEntry;
				if (FindDLL(filename, dllEntry))
				{
					if (dllEntry.last_write_time() > it.last_write_time())
						LoadDLL(it);
				}
				else
				{
					CompileCpp(it.path());
				}

				break;
			}

			if (!doesExist || isOutdated)
			{
				std::vector<GameObject*> addScriptObjects;
				if (doesExist)
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

				CompileCpp(it.path());
				bool success = LoadDLL(it);

				if (success)
				{
					for (auto& object : addScriptObjects)
					{
						Script* newScript = m_Scripts[filename]->Create();
						newScript->SetName(filename);
						object->AddScript(newScript);
					}
				}
			}
		}
	}

}

bool ScriptManager::FindDLL(const std::string& filename, std::filesystem::directory_entry& dllEntry)
{
	std::filesystem::path directory("Binaries");
	for (auto& entry : std::filesystem::directory_iterator(directory))
	{
		std::string dllname = GetFilename(entry.path());
		if (dllname == filename)
		{
			dllEntry = entry;
			return true;
		}
	}

	return false;
}

bool ScriptManager::LoadDLL(const std::filesystem::directory_entry& entry)
{
	std::string filename = GetFilename(entry.path());
	HINSTANCE dll = LoadLibraryA(("Binaries\\"s + filename + ".dll").c_str());

	if (dll)
	{
		std::pair<std::string, std::unique_ptr<ScriptDLL>> newElement;
		newElement = std::make_pair(filename, std::make_unique<ScriptDLL>(dll, filename, entry.last_write_time()));
		m_Scripts.insert(std::move(newElement));
		return true;
	}
	else
	{
		ERROR("Failed to load DLL!!!\n");
		return false;
	}
}

void ScriptManager::CompileCpp(const std::filesystem::path& filepath)
{
	std::string filename = GetFilename(filepath);
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
	compileCommand += " /EHsc /MDd /LD " + filepath.string();
#endif
#ifdef NDEBUG
	compileCommand += " /EHsc /MD /LD " + filepath.string();
#endif

	compileCommand += " /link Aspen.lib";

#ifdef _DEBUG
	compileCommand += " /LIBPATH:..\\x64\\Debug";
#endif
#ifdef NDEBUG
	compileCommand += " /LIBPATH:..\\x64\\Release";
#endif

	system(compileCommand.c_str());
	system(("move \""s + filename + ".dll\" \"\Binaries\"").c_str());
	system(("move \""s + filename + ".lib\" \"\Binaries\"").c_str());
	system(("move \""s + filename + ".obj\" \"\Binaries\"").c_str());
	system(("move \""s + filename + ".exp\" \"\Binaries\"").c_str());

}

std::string ScriptManager::GetFilename(const std::filesystem::path& path)
{
	std::string filepath = path.string();
	size_t lastDot = filepath.find_last_of('.');
	size_t end = filepath.find_last_of('\\') + 1;
	std::string filename(filepath.substr(end, lastDot - end));

	return filename;
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