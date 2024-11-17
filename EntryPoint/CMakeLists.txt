add_executable(alas_entry_point SandboxApp.cpp)


target_include_directories(alas_entry_point PRIVATE ../AlasCore/)

# target_link_libraries(alas_entry_point PRIVATE alas_engine_core)
target_link_libraries(alas_entry_point PRIVATE alas_engine_core)
target_include_directories(alas_entry_point PRIVATE ../AlasCore/vendor/spdlog/include)
target_include_directories(alas_entry_point PRIVATE ../AlasCore/src)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
target_include_directories(alas_entry_point PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(alas_engine_core PUBLIC ../AlasCore/vendor/OpenGL/GLAD/include/)
target_precompile_headers(alas_engine_core PUBLIC ../AlasCore/src/Core/precompiled.h)


if(WIN32)
  add_custom_command(
    TARGET alas_entry_point POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:SDL2::SDL2>"
      "$<TARGET_FILE_DIR:alas_entry_point>"
    VERBATIM
  )
  message("keke")
endif()