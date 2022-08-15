#pragma once
#include <string>

namespace File
{
	bool IsOfType(const std::string& filepath, const std::string& type);

	bool IsOfTypes(const std::string& filepath, std::initializer_list<std::string> types);
}