#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/Core/Logging.h"
#include <filesystem>

#ifdef VX_ENABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define VX_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { VX##type##ERROR(msg, __VA_ARGS__); VX_DEBUGBREAK(); } }
	#define VX_INTERNAL_ASSERT_WITH_MSG(type, check, ...) VX_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define VX_INTERNAL_ASSERT_NO_MSG(type, check) VX_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", VX_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define VX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define VX_INTERNAL_ASSERT_GET_MACRO(...) VX_EXPAND_MACRO( VX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, VX_INTERNAL_ASSERT_WITH_MSG, VX_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define VX_ASSERT(...) VX_EXPAND_MACRO( VX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define VX_CORE_ASSERT(...) VX_EXPAND_MACRO( VX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define VX_ASSERT(...)
	#define VX_CORE_ASSERT(...)
#endif