#include "Math.h"

float ToRads(float degrees)
{
	return degrees * (std::numbers::pi / 180.0f);
	//return degrees * (3.14f / 180.0f);
}

float ToDegrees(float rads)
{
	return rads * (180.0f / std::numbers::pi);
	//return rads * (180.0f / 3.14f);
}