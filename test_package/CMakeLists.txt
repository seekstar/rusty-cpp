cmake_minimum_required(VERSION 3.15)
project(test_package LANGUAGES CXX)

find_package(rusty-cpp REQUIRED CONFIG)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(${CMAKE_SOURCE_DIR} SRCS)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE rusty-cpp gtest)
