#pragma once

#include "Hurikan/Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Hurikan 
{
	class Log
	{
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	public:
		static void Init();
		static void Shutdown();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};

//Client log macros
#define HU_ERROR(...) ::Hurikan::Log::GetClientLogger()->error(__VA_ARGS__);
#define HU_WARN(...) ::Hurikan::Log::GetClientLogger()->warn(__VA_ARGS__);
#define HU_TRACE(...) ::Hurikan::Log::GetClientLogger()->trace(__VA_ARGS__);
#define HU_INFO(...) ::Hurikan::Log::GetClientLogger()->info(__VA_ARGS__);
#define HU_INFO(...) ::Hurikan::Log::GetClientLogger()->info(__VA_ARGS__);
#define HU_FATAL(...) ::Hurikan::Log::GetClientLogger()->critical(__VA_ARGS__);
//Core log macros
#define HU_CORE_ERROR(...) ::Hurikan::Log::GetCoreLogger()->error(__VA_ARGS__);
#define HU_CORE_WARN(...) ::Hurikan::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define HU_CORE_TRACE(...) ::Hurikan::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define HU_CORE_INFO(...) ::Hurikan::Log::GetCoreLogger()->info(__VA_ARGS__);
#define HU_CORE_FATAL(...) ::Hurikan::Log::GetCoreLogger()->critical(__VA_ARGS__);
}
