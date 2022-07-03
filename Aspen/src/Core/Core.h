#pragma once
#ifdef DLL_BUILD 
#define ASPEN __declspec(dllexport)
#else 
#define ASPEN __declspec(dllimport)
#endif