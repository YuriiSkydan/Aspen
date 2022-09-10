#include <any>
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
					{
						LoadDLL(it);
					}
					else
					{
						CompileCpp(it.path());
						LoadDLL(it);
					}
				}
				else
				{
					CompileCpp(it.path());
					LoadDLL(it);
				}

				continue;
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
		if (dllname == filename + "_generated")
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
	HINSTANCE dll = LoadLibraryA(("Binaries\\"s + filename + "_generated" + ".dll").c_str());

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
	std::string generatedFilename = filename + "_generated";

	std::ifstream scriptFile(filepath.string());
	std::ofstream generatedFile("Generated\\" + generatedFilename + ".cpp");

	if (!scriptFile.is_open())
	{
		while (!scriptFile.is_open())
		{
			scriptFile.open(filepath.string());
			ERROR("Failed to open script.");
		}

		INFO("Script Opened.");
	}

	if (!generatedFile.is_open())
	{
		ERROR("Failed to open generated script.");
		return;
	}

	std::string scriptCode;
	std::stringstream scriptStream;

	scriptStream << scriptFile.rdbuf();
	scriptFile.close();

	scriptCode = scriptStream.str();

	//Generating InitProperties Method
	//<name, type>
	std::unordered_map<std::string, std::string> properties;
	size_t propertyStartPos = 0;
	while (true)
	{
		propertyStartPos = scriptCode.find("[[SerializedField]]", propertyStartPos);
		if (propertyStartPos == std::string::npos)
			break;

		size_t typeStartPos = scriptCode.find_first_not_of(" ",
			propertyStartPos + "[[SerializedField]]"s.size() + 1);

		size_t typeEndPos = scriptCode.find_first_of(" ", typeStartPos);

		size_t nameStartPos = scriptCode.find_first_not_of(" ", typeEndPos + 1);
		size_t nameEndPos = scriptCode.find_first_of(" ", nameStartPos);

		std::string type = scriptCode.substr(typeStartPos, typeEndPos - typeStartPos);
		std::string name = scriptCode.substr(nameStartPos, nameEndPos - nameStartPos);

		propertyStartPos += "[[SerializedField]]"s.size() + 1;

		std::transform(type.begin(), type.end(), type.begin(), ::toupper);
		type = "VariableTypes::" + type;

		properties.insert({ name, type });
	}

	size_t startPos = scriptCode.find("GenerateBody()");
	startPos += "GenerateBody()"s.size();

	std::string initProperties;
	initProperties += "                      \n";
	initProperties += "public:               \n";
	initProperties += "void InitProperties() \n";
	initProperties += "{                     \n";

	for (auto& property : properties)
	{
		initProperties += "   m_Properties.push_back({ &" + property.first;
		initProperties += ", " + property.second;
		initProperties += ", \"" + property.first + "\"});\n";
	}

	initProperties += "}                     \n";

	scriptCode.insert(startPos, initProperties);

	//Generate Create Function
	scriptCode += "                                                          \n";
	scriptCode += "extern \"C\"                                              \n";
	scriptCode += "{                                                         \n";
	scriptCode += "   __declspec(dllexport) Script* Create"s + filename + "()\n";
	scriptCode += "   {                                                      \n";
	scriptCode += "       "s + filename + "* script = new " + filename + "();\n";
	scriptCode += "        script->InitProperties();                         \n";
	scriptCode += "        return script;                                    \n";
	scriptCode += "   }                                                      \n";
	scriptCode += "}                                                         \n";

	generatedFile << scriptCode;
	generatedFile.close();

	//Compiling generated cpp file
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
	compileCommand += " /EHsc /MDd /LD " + "Generated\\"s + generatedFilename + ".cpp";
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
	system(("move \""s + generatedFilename + ".dll\" \"\Binaries\"").c_str());
	system(("move \""s + generatedFilename + ".lib\" \"\Binaries\"").c_str());
	system(("move \""s + generatedFilename + ".obj\" \"\Binaries\"").c_str());
	system(("move \""s + generatedFilename + ".exp\" \"\Binaries\"").c_str());
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