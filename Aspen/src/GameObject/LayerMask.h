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

	uint8_t operator|=(const LayerMask& other);
	uint8_t operator&=(const LayerMask& other);

	uint8_t operator|=(uint8_t other);
	uint8_t operator&=(uint8_t other);

	friend uint8_t ASPEN operator|(const LayerMask& a, const LayerMask& b);
	friend uint8_t ASPEN operator&(const LayerMask& a, const LayerMask& b);

	friend uint8_t ASPEN operator|(const LayerMask& layer, uint8_t value);
	friend uint8_t ASPEN operator&(const LayerMask& layer, uint8_t value);

	friend uint8_t ASPEN operator|(uint8_t value, const LayerMask& layer);
	friend uint8_t ASPEN operator&(uint8_t value, const LayerMask& layer);
};

uint8_t ASPEN operator|(const LayerMask& a, const LayerMask& b);
uint8_t ASPEN operator&(const LayerMask& a, const LayerMask& b);

uint8_t ASPEN operator|(const LayerMask& layer, uint8_t value);
uint8_t ASPEN operator&(const LayerMask& layer, uint8_t value);

uint8_t ASPEN operator|(uint8_t value, const LayerMask& layer);
uint8_t ASPEN operator&(uint8_t value, const LayerMask& layer);