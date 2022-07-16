#include "Time.h"

void Time::FrameStart()
{
	start = std::chrono::high_resolution_clock::now();
}

void Time::FrameEnd()
{
	auto end = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<double>(end - start).count();
}
