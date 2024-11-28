add_executable(alas_entry_point SandboxApp.cpp)


target_include_directories(alas_entry_point PRIVATE ../AlasCore/)

# target_link_libraries(alas_entry_point PRIVATE alas_engine_core)
target_link_libraries(alas_entry_point PRIVATE alas_engine_core)
target_include_directories(alas_entry_point PRIVATE ../AlasCore/vendor/spdlog/include)
target_include_directories(alas_entry_point PRIVATE ../AlasCore/src)

if (SDL_VENDORED)
    target_include_directories(alas_entry_point PUBLIC ../AlasCore/vendor/SDL/include/)
else()
    find_package(SDL3 REQUIRED)
    target_include_directories(alas_entry_point PUBLIC ${SDL3_INCLUDE_DIRS})
endif()

find_package(OpenGL REQUIRED)

target_include_directories(alas_engine_core PUBLIC ../AlasCore/vendor/OpenGL/GLAD/include/)
target_include_directories(alas_engine_core PUBLIC ../AlasCore/vendor/imgui/)

target_precompile_headers(alas_engine_core PUBLIC ../AlasCore/src/Core/precompiled.h)


if(WIN32)
  add_custom_command(
    TARGET alas_entry_point POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:SDL3::SDL3>"
      "$<TARGET_FILE_DIR:alas_entry_point>"
    VERBATIM
  )
  message("keke")
endif()