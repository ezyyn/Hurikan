#pragma once

#include "Hurikan/Core/Base.h"
#include "Hurikan/Core/Log.h"
#include <filesystem>

#ifdef HU_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define HU_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { HU##type##ERROR(msg, __VA_ARGS__); HU_DEBUGBREAK(); } }
#define HU_INTERNAL_ASSERT_WITH_MSG(type, check, ...) HU_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define HU_INTERNAL_ASSERT_NO_MSG(type, check) HU_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", HU_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define HU_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define HU_INTERNAL_ASSERT_GET_MACRO(...) HU_EXPAND_MACRO( HU_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HU_INTERNAL_ASSERT_WITH_MSG, HU_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define HU_ASSERT(...) HU_EXPAND_MACRO( HU_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define HU_CORE_ASSERT(...) HU_EXPAND_MACRO( HU_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define HU_ASSERT(...)
#define HU_CORE_ASSERT(...)
#endif