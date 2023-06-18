cmake_minimum_required(VERSION 3.15)
project(rusty-cpp CXX)

add_library(rusty-cpp INTERFACE)
target_include_directories(rusty-cpp INTERFACE include)

file(GLOB_RECURSE RUSTY_HEADERS "include/rusty/*")
set_target_properties(rusty-cpp PROPERTIES PUBLIC_HEADER "${RUSTY_HEADERS}")

install(TARGETS rusty-cpp DESTINATION "include/rusty")
