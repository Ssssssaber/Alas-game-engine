#pragma once


#define BIT(x) (1 << x)
#define FMT_UNICODE 0

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
#include <random>

#define MIN_ID 1
#define MAX_ID UINT64_MAX
namespace Alas {
    template <typename T>
    using Shared = std::shared_ptr<T>;

    template <typename T>
    using Unique = std::unique_ptr<T>; 

    using UID = uint64_t;

    static std::random_device s_RandomDevice;  // a seed source for the random number engine
    static std::mt19937 s_RandomGenerator(s_RandomDevice()); // mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution(MIN_ID, MAX_ID);

    static UID GetUniqueId()
    {
        return s_UniformDistribution(s_RandomGenerator);
    }
    
};