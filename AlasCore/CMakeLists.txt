
if (AGS_ENABLE_ASSERTS)
    add_definitions(-DAGS_ENABLE_ASSERTS)
endif()

if (SDL_WINDOWS)
    add_definitions(-DSDL_WINDOWS)
endif()

if(SDL_VENDORED)
    add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()

find_package(OpenGL)

set(IMGUI_SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp)

set(IMGUI_BACKEND
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_sdl2.cpp)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    ${IMGUI_SOURCES}
    ${IMGUI_BACKEND}    
)

add_library(alas_engine_core SHARED ${SOURCES})
target_include_directories(alas_engine_core PUBLIC vendor/ImGui/ vendor/ImGui/backends)

target_include_directories(alas_engine_core PUBLIC src)

add_subdirectory(vendor/OpenGL/GLAD/)
add_dependencies(alas_engine_core glad_lib)

target_link_libraries(alas_engine_core PUBLIC glad_lib)

add_subdirectory(vendor/spdlog)
add_dependencies(alas_engine_core spdlog)
target_link_libraries(alas_engine_core PUBLIC  spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(alas_engine_core PUBLIC SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(alas_engine_core PUBLIC SDL2::SDL2)
