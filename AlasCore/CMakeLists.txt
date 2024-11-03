file(GLOB SOURCES
    src/Core/*.h
    src/Core/*.cpp
    src/Events/*.h
    src/Events/*.cpp
    Alas.h
)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/sdl2)

add_library(alas_engine_core SHARED ${SOURCES})
target_include_directories(alas_engine_core PUBLIC src)

add_dependencies(alas_engine_core spdlog)
target_link_libraries(alas_engine_core PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

target_link_libraries(alas_engine_core PRIVATE SDL2)
