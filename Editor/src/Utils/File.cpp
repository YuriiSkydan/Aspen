#include "File.h"

bool File::IsOfType(const std::string& filepath, const std::string& type)
{
	size_t lastDotIndex = filepath.find_last_of(".");

	if (lastDotIndex != std::string::npos)
	{
		std::string fileType = filepath.substr(lastDotIndex, filepath.size() - lastDotIndex);

		return fileType == type;
	}

	return false;
}

bool File::IsOfTypes(const std::string& filepath, std::initializer_list<std::string> types)
{
	size_t lastDotIndex = filepath.find_last_of(".");

	if (lastDotIndex != std::string::npos)
	{
		std::string fileType = filepath.substr(lastDotIndex, filepath.size() - lastDotIndex);

		for (auto& type : types)
		{
			if (fileType == type)
				return true;
		}
	}

	return false;
}