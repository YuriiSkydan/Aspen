#pragma once
#include <unordered_map>
#include <string_view>
#include "../Core/Core.h"

class ASPEN LayerMask
{
private:
	inline static std::unordered_map <std::uint8_t, std::string> s_Layers;

	std::uint8_t m_Value;

	friend class Inspector;
private:
	static void Add(std::string_view name);
	static void Delete(std::string_view name);
	static const std::unordered_map<std::uint8_t, std::string>& GetLayers() { return s_Layers; }

public:
	LayerMask();
	LayerMask(std::uint8_t value);
	static LayerMask GetMask(std::string_view name);
	static const std::string& LayerToName(std::uint8_t value);
};

LayerMask operator|(const LayerMask& a, const LayerMask& b);
LayerMask operator&(const LayerMask& a, const LayerMask& b);