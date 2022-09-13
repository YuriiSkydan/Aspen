#include "LayerMask.h"

void LayerMask::Add(std::string_view name)
{
	s_Layers.insert({ s_Layers.size(), name.data() });
}

void LayerMask::Remove(std::string_view name)
{
	for (auto it = s_Layers.cbegin(); it != s_Layers.cend(); ++it)
	{
		if (it->second == name.data())
			s_Layers.erase(it);
	}
}

LayerMask::LayerMask()
	: m_Value(0)
{ }

LayerMask::LayerMask(std::uint8_t value)
	: m_Value(value)
{ }

LayerMask LayerMask::GetMask(std::string_view name)
{
	for (auto& layer : s_Layers)
	{
		if (layer.second == name.data())
			return layer.first;
	}

	return 0;
}

const std::string& LayerMask::LayerToName(std::uint8_t value)
{
	auto name = s_Layers.find(value);
	if (name != s_Layers.end())
	{
		return name->second;
	}

	return " ";
}

uint8_t LayerMask::operator|=(const LayerMask& other)
{
	m_Value |= other.m_Value;
	return m_Value;
}

uint8_t LayerMask::operator&=(const LayerMask& other)
{
	m_Value &= other.m_Value;
	return m_Value;
}

uint8_t LayerMask::operator|=(uint8_t other)
{
	m_Value |= other;
	return m_Value;
}

uint8_t LayerMask::operator&=(uint8_t other)
{
	m_Value &= other;
	return m_Value;
}

uint8_t operator|(const LayerMask& a, const LayerMask& b)
{
	return (a.m_Value | b.m_Value);
}

uint8_t operator&(const LayerMask& a, const LayerMask& b)
{
	return (a.m_Value & b.m_Value);
}

uint8_t operator|(const LayerMask& layer, uint8_t value)
{
	return (layer.m_Value | value);
}

uint8_t operator&(const LayerMask& layer, uint8_t value)
{
	return (layer.m_Value & value);
}

uint8_t operator|(uint8_t value, const LayerMask& layer)
{
	return (value | layer.m_Value);
}

uint8_t operator&(uint8_t value, const LayerMask& layer)
{
	return (value & layer.m_Value);
}