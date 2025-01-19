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

add_custom_command(
  TARGET alas_entry_point
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "${FullOutputDir}/Assets"
  COMMENT "---- Copying Assets folder from EntryPoint -----"
)