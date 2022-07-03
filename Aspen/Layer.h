#pragma once
#include <string>

#include "src/Core/Core.h"

class ASPEN Layer
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

