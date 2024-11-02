file(GLOB SOURCES
    src/Core/*.h
    src/Core/*.cpp
)


add_library(alas_engine_core SHARED ${SOURCES})

include_directories(src)
