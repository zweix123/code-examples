cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 20) # C++ version
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++ version must be required
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message("Setting default build type to Release")
endif()

project(
  code-examples
  VERSION 0.0.1
  LANGUAGES C CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(test)
add_subdirectory(misc)
add_subdirectory(obj_print/test)
add_subdirectory(cache/test)
add_subdirectory(hash_table/test)
