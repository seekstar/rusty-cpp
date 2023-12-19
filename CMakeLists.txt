cmake_minimum_required(VERSION 3.15)
project(rusty-cpp CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE include)

file(GLOB_RECURSE HEADERS "include/rusty/*")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ${PROJECT_NAME} DESTINATION "include/rusty")
