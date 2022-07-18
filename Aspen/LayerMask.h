#pragma once
#include <unordered_map>
#include <string_view>

class LayerMask
{
private:
	inline static std::unordered_map <std::uint8_t, std::string> m_Layers;

	std::pair<std::uint8_t, std::string> m_Layer;
public:

	static std::uint8_t GetMask(std::string_view name);
	 
};

LayerMask operator|(const LayerMask& a, const LayerMask& b);
LayerMask operator&(const LayerMask& a, const LayerMask& b);