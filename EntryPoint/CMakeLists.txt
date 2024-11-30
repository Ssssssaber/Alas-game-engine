add_executable(alas_entry_point SandboxApp.cpp)


target_include_directories(alas_entry_point PRIVATE ${PROJECT_SOURCE_DIR}/AlasCore/)

# target_link_libraries(alas_entry_point PRIVATE alas_engine_core)
target_link_libraries(alas_entry_point PRIVATE alas_engine_core)

target_include_directories(alas_engine_core PRIVATE 
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/AlasCore/src
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/OpenGL/GLAD/include/
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/imgui/
    ${PROJECT_SOURCE_DIR}/AlasCore/vendor/glm/glm/
)

target_precompile_headers(alas_engine_core PUBLIC ${PROJECT_SOURCE_DIR}/AlasCore/src/Core/precompiled.h)

if (SDL_VENDORED)
    target_include_directories(alas_entry_point PRIVATE ${PROJECT_SOURCE_DIR}/AlasCore/vendor/SDL/include/)
else()
    find_package(SDL3 REQUIRED)
    target_include_directories(alas_entry_point PRIVATE ${SDL3_INCLUDE_DIRS})
endif()

find_package(OpenGL REQUIRED)

if(WIN32)
  add_custom_command(
    TARGET alas_entry_point POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:SDL3::SDL3>"
      "$<TARGET_FILE_DIR:alas_entry_point>"
    VERBATIM
  )
endif()