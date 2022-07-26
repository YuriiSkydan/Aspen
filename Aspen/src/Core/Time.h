#pragma once
#include <chrono>

#include "Core.h"

class ASPEN Time
{
private:
	inline static double m_FixedDeltaTime = 0.015f;
	inline static double m_DeltaTime = 0;

	inline static std::chrono::high_resolution_clock::time_point start;

	friend class Engine;
private:
	static void FrameStart();
	static void FrameEnd();

public:
	static double DeltaTime() { return m_DeltaTime; }
	static double FixedDeltaTime() { return m_FixedDeltaTime; }
};