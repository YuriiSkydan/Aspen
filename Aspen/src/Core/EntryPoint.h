#pragma once

extern Engine* CreateEngine();
extern void AddEditor();

int main()
{
	Engine* engine = CreateEngine();

	engine->Run();

	delete engine;

	return 0;
}
