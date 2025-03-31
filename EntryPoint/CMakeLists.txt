file(GLOB_RECURSE SOURCES
    CustomScripts/*.cpp
    CustomScripts/*.h
    SandboxApp.cpp
)

add_executable(alas_entry_point ${SOURCES})

target_include_directories(alas_entry_point PRIVATE ${PROJECT_SOURCE_DIR}/AlasCore/)

target_link_libraries(alas_entry_point PRIVATE alas_engine_core)

if(WIN32)
  add_custom_command(
    TARGET alas_entry_point POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:SDL3::SDL3>"
      "$<TARGET_FILE_DIR:alas_entry_point>"
    VERBATIM
  )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (ASSETS_OUTPUT_DIR "${FULL_OUTPUT_DIR}/Assets/")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set (ASSETS_OUTPUT_DIR "${FULL_OUTPUT_DIR}${CMAKE_BUILD_TYPE}/Assets/")
endif()

message("---- Assets output dir: ${ASSETS_OUTPUT_DIR}")

# add_custom_command(
#   TARGET alas_entry_point
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND}
#   ARGS -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/Assets/" "${ASSETS_OUTPUT_DIR}"
#   COMMENT "---- Copying Assets folder from EntryPoint -----"
# )