

file(GLOB_RECURSE GLAD_SOURCES
    src/*.c
    include/*.h
)

add_library(glad_lib ${GLAD_SOURCES})
# include_directories(include)
target_include_directories(glad_lib PUBLIC include)