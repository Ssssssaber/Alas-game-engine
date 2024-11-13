#pragma once

#define BIT(x) (1 << x)

#ifdef AGS_ENABLE_ASSERTS
	#define AGS_ASSERT(x, ...) { if(!(x)) { AGS_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AGS_CORE_ASSERT(x, ...) { if(!(x)) { AGS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AGS_ASSERT(x, ...)
	#define AGS_CORE_ASSERT(x, ...)
#endif