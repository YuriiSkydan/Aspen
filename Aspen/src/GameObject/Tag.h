#pragma once
#include <vector>
#include <string>
#include <string_view>

class Tag
{
private:
	inline static std::vector<std::string> s_Tags {"Untagged"};

	std::string m_Name;
private:
	static void Add(std::string_view name);
	static void Remove(std::string_view name);
	static const std::vector<std::string>& GetTags() { return s_Tags; }

public:
	Tag();

	friend bool operator==(const Tag& a, const Tag& b);
	friend bool operator!=(const Tag& a, const Tag& b);

	friend bool operator==(const std::string& name, const Tag& tag);
	friend bool operator!=(const std::string& name, const Tag& tag);

	friend bool operator==(const Tag& tag, const std::string& name);
	friend bool operator!=(const Tag& tag, const std::string& name);
};

bool operator==(const Tag& a, const Tag& b);
bool operator!=(const Tag& a, const Tag& b);

bool operator==(const std::string& name, const Tag& tag);
bool operator!=(const std::string& name, const Tag& tag);

bool operator==(const Tag& tag, const std::string& name);
bool operator!=(const Tag& tag, const std::string& name);