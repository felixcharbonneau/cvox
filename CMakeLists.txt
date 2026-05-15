cmake_minimum_required(VERSION 3.21)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER gcc-14)
set(CMAKE_CXX_COMPILER g++-14)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/deps.cmake")

project(cvox
    VERSION 1.0
)

add_subdirectory(cvox)

add_subdirectory(sandbox)
