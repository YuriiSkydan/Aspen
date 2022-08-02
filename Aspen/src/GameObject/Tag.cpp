#include <algorithm>
#include "Tag.h"

void Tag::Add(std::string_view name)
{
	s_Tags.push_back(name.data());
}

void Tag::Remove(std::string_view name)
{
	std::erase_if(s_Tags, [&](std::string_view tag)
	{
		if (tag == name)
			return true;

		return false;
	});
}

Tag::Tag()
{
	m_Name = s_Tags[0];
}

bool operator==(const Tag& a, const Tag& b)
{
	return a.m_Name == b.m_Name;
}

bool operator!=(const Tag& a, const Tag& b)
{
	return a.m_Name != b.m_Name;
}

bool operator==(const std::string& name, const Tag& tag)
{
	return name == tag.m_Name;
}

bool operator!=(const std::string& name, const Tag& tag)
{
	return name != tag.m_Name;
}

bool operator==(const Tag& tag, const std::string& name)
{
	return tag.m_Name == name;
}

bool operator!=(const Tag& tag, const std::string& name)
{
	return tag.m_Name != name;
}