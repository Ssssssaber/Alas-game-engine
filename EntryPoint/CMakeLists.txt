add_executable(alas_entry_point SandboxApp.cpp)


target_include_directories(alas_entry_point PUBLIC ../AlasCore/)

target_link_libraries(alas_entry_point PUBLIC ${FullOutputDir}/libalas_engine_core.dll)

target_include_directories(alas_entry_point PUBLIC ../AlasCore/vendor/spdlog/include)
target_include_directories(alas_entry_point PUBLIC ../AlasCore/src)
