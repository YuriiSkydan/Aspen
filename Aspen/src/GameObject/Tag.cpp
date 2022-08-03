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
			return name == tag;
		});
}

Tag::Tag(std::string_view name)
	: m_Name(name)
{ }

Tag::Tag()
{
	m_Name = s_Tags[0];
}

Tag Tag::GetTag(std::string_view name)
{
	auto it = std::find_if(s_Tags.begin(), s_Tags.end(),
		[&](std::string_view tag)
		{
			if (tag == name)
				return true;

			return false;
		});

	if (it == s_Tags.end())
		return Tag(s_Tags[0]);

	return Tag(*it);
}

bool operator==(const Tag& a, const Tag& b)
{
	return a.m_Name == b.m_Name;
}

bool operator!=(const Tag& a, const Tag& b)
{
	return a.m_Name != b.m_Name;
}

bool operator==(std::string_view name, const Tag& tag)
{
	return name == tag.m_Name;
}

bool operator!=(std::string_view name, const Tag& tag)
{
	return name != tag.m_Name;
}

bool operator==(const Tag& tag, std::string_view name)
{
	return tag.m_Name == name;
}

bool operator!=(const Tag& tag, std::string_view name)
{
	return tag.m_Name != name;
}