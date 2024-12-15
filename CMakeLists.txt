cmake_minimum_required(VERSION 3.12)

project(LyndonArrays LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(LyndonArrays
    src/main.cpp         # Main source file
    src/inverse-lyndon.hpp # Header file
)

target_include_directories(LyndonArrays PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
