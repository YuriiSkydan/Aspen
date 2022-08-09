#include "Time.h"

void Time::FrameStart()
{
	auto end = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<double>(end - start).count();
	start = std::chrono::high_resolution_clock::now();
}