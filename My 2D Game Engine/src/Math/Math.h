#pragma once
#include <cmath>
#include <numbers>

float ToRads(float degrees)
{
	return degrees * (std::numbers::pi / 180.0f);
}

float ToDegrees(float rads)
{
	return rads * (180.0f / std::numbers::pi);
}