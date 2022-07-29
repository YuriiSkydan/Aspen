#pragma once

extern Engine* CreateEngine();
extern void AddEditor();

int main()
{
	Engine* engine = CreateEngine();
	//Engine* engine = new Engine;
	//AddEditor();

	engine->Run();

	delete engine;

	return 0;
}
