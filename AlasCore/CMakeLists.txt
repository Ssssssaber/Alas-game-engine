file(GLOB SOURCES
    src/Core/*.h
    src/Core/*.cpp
    alas.h
)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_library(alas_engine_core SHARED ${SOURCES})
target_include_directories(alas_engine_core PUBLIC src)
add_dependencies(alas_engine_core spdlog)

add_subdirectory(vendor/spdlog)
target_include_directories(alas_engine_core PUBLIC vendor/spdlog/include)
# target_link_libraries(alas_engine_core PRIVATE ${FullOutputDir}/static/libspdlog.a)
target_link_libraries(alas_engine_core PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
