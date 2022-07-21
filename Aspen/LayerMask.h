#pragma once
#include <unordered_map>
#include <string_view>

class LayerMask
{
private:
	inline static std::unordered_map <std::uint8_t, std::string> m_Layers;

	std::uint8_t m_Value;
private:
	void Add(std::string_view name);

public:

	static LayerMask GetMask(std::string_view name);
	static std::uint8_t NameToLayer(std::string_view name);
	static const std::string& LayerToName(std::uint8_t);
};

LayerMask operator|(const LayerMask& a, const LayerMask& b);
LayerMask operator&(const LayerMask& a, const LayerMask& b);