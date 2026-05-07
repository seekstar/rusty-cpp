cmake_minimum_required(VERSION 3.15)
project(test_package LANGUAGES CXX)

add_compile_options(-Wall -Wextra)

option(USE_PARENT OFF)
if (USE_PARENT)
	add_subdirectory(.. ${CMAKE_BINARY_DIR}/rusty-cpp)
else()
	find_package(rusty-cpp REQUIRED CONFIG)
	# CMake treats IMPORTED target include dirs as system headers (-isystem).
	# Override so that warnings in rusty-cpp headers are visible during testing.
	set_target_properties(rusty-cpp PROPERTIES IMPORTED_NO_SYSTEM TRUE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(${SRCS})
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE rusty-cpp gtest)

# target_compile_features can omit -std flag in compile_commands.json
# https://gitlab.kitware.com/cmake/cmake/-/issues/23397
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
