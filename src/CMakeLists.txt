project(meteod CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

file(GLOB SRC_LIST *.cpp protocols/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ./ protocols/)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror)
