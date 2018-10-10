project(meteod)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
add_subdirectory(src)

enable_testing()

if(BUILD_TESTING)
    add_subdirectory (tests)
endif()