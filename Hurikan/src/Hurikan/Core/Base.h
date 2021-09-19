#pragma once

#include <memory>

#ifdef HU_PLATFORM_WINDOWS
	
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator not supported!"
	#endif
	#error Hurikan ony supports Windows! 
#endif

#ifdef HU_DEBUG
	#define HU_ENABLE_ASSERTS
#endif

#define HU_CORE_ASSERT(x,...) if(!(x)){ HU_CORE_ERROR(__VA_ARGS__); __debugbreak();}
#ifdef HU_ENABLE_ASSERTS
	//#define HU_CORE_ASSERT(x,...) if(!(x)){ HU_CORE_ERROR(__VA_ARGS__); __debugbreak();}
#endif

namespace Hurikan {
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