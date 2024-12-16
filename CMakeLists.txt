cmake_minimum_required(VERSION 3.10.0)
set (CMAKE_CXX_STANDARD 17)
project(alas_game_engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

option(SDL_VENDORED "Use vendored SDL library" ON)
option(SDL_WINDOWS "Use SDL2 for window creation" ON)
option(ALAS_ENABLE_ASSERTS "Use assertions during app running" ON)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()
set (FullOutputDir "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/static")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")


add_subdirectory(AlasCore)
add_subdirectory(EntryPoint)
add_dependencies(alas_entry_point alas_engine_core)
