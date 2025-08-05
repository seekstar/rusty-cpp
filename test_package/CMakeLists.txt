cmake_minimum_required(VERSION 3.15)
project(test_package LANGUAGES CXX)

find_package(rusty-cpp REQUIRED CONFIG)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} test_package.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE rusty-cpp gtest)
