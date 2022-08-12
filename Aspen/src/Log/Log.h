#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <string>

#include "../Core/Core.h"

class ASPEN Log
{
private:
	inline static std::shared_ptr<spdlog::logger> s_CoreLogger;
	inline static std::shared_ptr<spdlog::logger> s_ClientLogger;

public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
};

#define INFO(...)    Log::GetCoreLogger()->info(__VA_ARGS__) 
#define WARN(...)    Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ERROR(...)   Log::GetCoreLogger()->error(__VA_ARGS__)
#define TRACE(...)   Log::GetCoreLogger()->trace(__VA_ARGS__) 
#define FATAL(...)   Log::GetCoreLogger()->fatal(__VA_ARGS__)
