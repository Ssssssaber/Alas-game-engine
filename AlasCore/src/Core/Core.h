#pragma once

#define BIT(x) (1 << x)

#ifdef ALAS_ENABLE_ASSERTS
	
	#ifdef WIN32
		#define ALAS_ASSERT(x, ...) { if(!(x)) { ALAS_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define ALAS_CORE_ASSERT(x, ...) { if(!(x)) { ALAS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#endif
	
	#ifdef __linux__
		#include <signal.h>
		#define ALAS_ASSERT(x, ...) { if(!(x)) { ALAS_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
		#define ALAS_CORE_ASSERT(x, ...) { if(!(x)) { ALAS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
	#endif

	#ifdef __APPLE__
		throw("Alas game engine does not support MacOS");
	#endif
#else
	#define ALAS_ASSERT(x, ...)
	#define ALAS_CORE_ASSERT(x, ...)
#endif

#include <memory>

namespace Alas {
    template <typename T>
    using Shared = std::shared_ptr<T>;

    template <typename T>
    using Unique = std::unique_ptr<T>; 
};