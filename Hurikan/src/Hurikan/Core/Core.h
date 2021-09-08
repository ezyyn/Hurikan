#pragma once

#include <memory>

#ifdef HU_PLATFORM_WINDOWS
	#if HU_DYNAMIC_LINK
		#ifdef HU_BUILD_DLL
			#define HU_API __declspec(dllexport)
		#else 
			#define HU_API __declspec(dllimport) 
		#endif 
	#else 
		#define HU_API
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator not supported!"
	#endif
	#error Hurikan ony supports Windows! 
#endif

#define HU_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

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