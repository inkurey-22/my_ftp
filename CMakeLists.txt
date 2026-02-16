
cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER clang)

project(myftp LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES src/*.c)

add_executable(myftp ${SRC_FILES})

set_target_properties(myftp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_include_directories(myftp PRIVATE include)
