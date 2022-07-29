#include "../Aspen/src/Engine/Engine.h"
#include "src/Editor/Editor.h"
#include "src/Core/EntryPoint.h"

Engine* CreateEngine()
{
	Engine* engine = new Engine;

	std::unique_ptr<Layer> editor = std::make_unique<Editor>();

	engine->AddEditor(std::move(editor));

	return engine;
}

void AddEditor()
{
	std::unique_ptr<Layer> editor = std::make_unique<Editor>();

	Engine::Get().AddEditor(std::move(editor));
}

//int main()
//{
//	/*auto engine = new Engine;
//	
//	std::unique_ptr<Layer> editor = std::make_unique<Editor>();
//	engine->AddEditor(std::move(editor));
//	std::cout << "Editor Window: " << Engine::Get().GetWindow().GetNativeWindow() << std::endl;
//	engine->Run();
//
//	delete engine;*/
//}