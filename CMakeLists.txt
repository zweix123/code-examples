cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 17)  # C++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # C++ version must be required
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

project(code-exmaples VERSION 0.0.1 LANGUAGES C CXX)

add_subdirectory(./test)
