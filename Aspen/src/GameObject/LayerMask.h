#pragma once
#include <unordered_map>
#include <string_view>
#include "../Core/Core.h"

class ASPEN LayerMask
{
private:
	inline static std::unordered_map <std::uint8_t, std::string> s_Layers{ {0, "Default"} };

	std::uint8_t m_Value = 0;

	friend class Inspector;
private:
	static void Add(std::string_view name);
	static void Remove(std::string_view name);
	static std::unordered_map<std::uint8_t, std::string>& GetLayers() { return s_Layers; }

public:
	LayerMask();
	LayerMask(std::uint8_t value);
	static LayerMask GetMask(std::string_view name);
	static const std::string& LayerToName(std::uint8_t value);

	friend LayerMask operator|(const LayerMask& a, const LayerMask& b);
	friend LayerMask operator&(const LayerMask& a, const LayerMask& b);
	
	friend LayerMask operator|(const LayerMask& layer, std::uint8_t value);
	friend LayerMask operator&(const LayerMask& layer, std::uint8_t value);
	
	friend LayerMask operator|(std::uint8_t value, const LayerMask& layer);
	friend LayerMask operator&(std::uint8_t value, const LayerMask& layer);
};

LayerMask operator|(const LayerMask& a, const LayerMask& b);
LayerMask operator&(const LayerMask& a, const LayerMask& b);

LayerMask operator|(const LayerMask& layer, std::uint8_t value);
LayerMask operator&(const LayerMask& layer, std::uint8_t value);

LayerMask operator|(std::uint8_t value, const LayerMask& layer);
LayerMask operator&(std::uint8_t value, const LayerMask& layer);