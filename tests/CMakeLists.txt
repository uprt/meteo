project(meteod-tests)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(SRC_DIR "../src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(${PROJECT_NAME}
                ${SRC_DIR}/protocols/proto1.cpp
                proto1-test.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/protocols
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_NAME} ${GTEST_LIBRARIES} Threads::Threads)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

