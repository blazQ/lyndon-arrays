# Minimum required CMake version
cmake_minimum_required(VERSION 3.12)

# Project name and language
project(LyndonArrays LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(LyndonArrays
    src/main.cpp         # Main source file
    src/inverse-lyndon.hpp # Header file
)

# Include directories for header files
target_include_directories(LyndonArrays PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
