cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(matcha LANGUAGES CXX)

add_executable(matcha src/main.cpp)

set_target_properties(matcha PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
