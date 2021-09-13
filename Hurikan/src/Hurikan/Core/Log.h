#pragma once
#include "hupch.h"

#include "Hurikan/Core/Base.h"
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace Hurikan {

	class HU_API Log
	{
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};

//Additional macros
#define HU_EXECUTE_ONCE(func)
//Client log macros
#define HU_ERROR(...) ::Hurikan::Log::GetClientLogger()->error(__VA_ARGS__);
#define HU_WARN(...) ::Hurikan::Log::GetClientLogger()->warn(__VA_ARGS__);
#define HU_TRACE(...) ::Hurikan::Log::GetClientLogger()->trace(__VA_ARGS__);
#define HU_INFO(...) ::Hurikan::Log::GetClientLogger()->info(__VA_ARGS__);
#define HU_FATAL(...) ::Hurikan::Log::GetClientLogger()->critical(__VA_ARGS__);
//Core log macros
#define HU_CORE_ERROR(...) ::Hurikan::Log::GetCoreLogger()->error(__VA_ARGS__);
#define HU_CORE_WARN(...) ::Hurikan::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define HU_CORE_TRACE(...) ::Hurikan::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define HU_CORE_INFO(...) ::Hurikan::Log::GetCoreLogger()->info(__VA_ARGS__);
#define HU_CORE_FATAL(...) ::Hurikan::Log::GetCoreLogger()->critical(__VA_ARGS__);
#define HU_CORE_ASSERT(x,...) if(!(x)){ HU_CORE_ERROR(__VA_ARGS__); __debugbreak();}
}

