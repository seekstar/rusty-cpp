cmake_minimum_required(VERSION 3.15)
project(rusty-cpp CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE include)

install(DIRECTORY include/ DESTINATION "include")
