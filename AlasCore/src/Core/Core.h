#pragma once

#define BIT(x) (1 << x)

#ifdef AGS_ENABLE_ASSERTS
	
	#ifdef WIN32
		#define AGS_ASSERT(x, ...) { if(!(x)) { AGS_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define AGS_CORE_ASSERT(x, ...) { if(!(x)) { AGS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#endif
	
	#ifdef __linux__
		#include <signal.h>
		#define AGS_ASSERT(x, ...) { if(!(x)) { AGS_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
		#define AGS_CORE_ASSERT(x, ...) { if(!(x)) { AGS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
	#endif

	#ifdef __APPLE__
		throw("Alas game engine does not support MacOS");
	#endif
#else
	#define AGS_ASSERT(x, ...)
	#define AGS_CORE_ASSERT(x, ...)
#endif
