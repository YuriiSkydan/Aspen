#pragma once
#include <string>

class Layer
{
private:
	std::string m_Name;

public:
	Layer(const std::string& name = "Layer");

	virtual void Attach() {}
	virtual void Detach() {}
	virtual void Update() {}
	virtual void ImGuiRender() {}
	
	//TO DO: OnEvent method

};

