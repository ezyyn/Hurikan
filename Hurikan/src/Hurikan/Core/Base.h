#pragma once

#include <memory>

#include "Hurikan/Core/PlatformDetection.h"

#ifdef HU_DEBUG
	#if defined(HU_PLATFORM_WINDOWS)
		#define HU_DEBUGBREAK() __debugbreak()
	#elif defined(HU_PLATFORM_LINUX)
		#include <signal.h>
		#define HU_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define HU_ENABLE_ASSERTS
#else
	#define HZ_DEBUGBREAK()
#endif

#define HU_EXPAND_MACRO(x) x
#define HU_STRINGIFY_MACRO(x) #x
#define HU_CORE_ASSERT(x,...) if(!(x)){ HU_CORE_ERROR(__VA_ARGS__); __debugbreak();}

#define BIT(x) (1 << x)

#define HU_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Hurikan 
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args> 
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}