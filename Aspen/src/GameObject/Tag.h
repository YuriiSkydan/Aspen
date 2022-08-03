#pragma once
#include <vector>
#include <string>
#include <string_view>
#include "../Core/Core.h"

class ASPEN Tag
{
private:
	inline static std::vector<std::string> s_Tags {"Untagged"};

	std::string m_Name;

	friend class Inspector;
private:
	static void Add(std::string_view name);
	static void Remove(std::string_view name);
	static std::vector<std::string>& GetTags() { return s_Tags; }

	Tag(std::string_view name);
public:
	Tag();

	const std::string GetName() const { return m_Name; }
	
	static Tag GetTag(std::string_view name);

	friend bool operator==(const Tag& a, const Tag& b);
	friend bool operator!=(const Tag& a, const Tag& b);

	friend bool operator==(std::string_view name, const Tag& tag);
	friend bool operator!=(std::string_view name, const Tag& tag);

	friend bool operator==(const Tag& tag, std::string_view name);
	friend bool operator!=(const Tag& tag, std::string_view name);
};

bool operator==(const Tag& a, const Tag& b);
bool operator!=(const Tag& a, const Tag& b);

bool operator==(std::string_view name, const Tag& tag);
bool operator!=(std::string_view name, const Tag& tag);

bool operator==(const Tag& tag, std::string_view name);
bool operator!=(const Tag& tag, std::string_view name);