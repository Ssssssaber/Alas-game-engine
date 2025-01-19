if (ALAS_ENABLE_ASSERTS)
    add_definitions(-DALAS_ENABLE_ASSERTS)
endif()

if (SDL_WINDOWS)
    add_definitions(-DSDL_WINDOWS)
endif()

set(IMGUI_SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/misc/cpp/imgui_stdlib.cpp
)

set(IMGUI_BACKEND
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_sdl3.cpp
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    ${IMGUI_SOURCES}
    ${IMGUI_BACKEND}
)

add_library(alas_engine_core ${SOURCES})

target_include_directories(alas_engine_core PUBLIC
    ${PROJECT_SOURCE_DIR}/AlasCore/
    ${PROJECT_SOURCE_DIR}/AlasCore/src
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/OpenGL/GLAD/include/
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/imgui/
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/imgui/backends
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/glm/glm/
)

target_precompile_headers(alas_engine_core PUBLIC ${PROJECT_SOURCE_DIR}/AlasCore/src/Core/precompiled.h)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/OpenGL/GLAD/)
if(SDL_VENDORED)
    add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3main)
endif()

add_dependencies(alas_engine_core 
    glad_lib
    spdlog
)

find_package(OpenGL)

# SDL3::SDL3main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL3::SDL3main)
    target_link_libraries(alas_engine_core PUBLIC SDL3::SDL3main)
endif()

target_link_libraries(alas_engine_core PUBLIC 
    glad_lib
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    SDL3::SDL3
)

