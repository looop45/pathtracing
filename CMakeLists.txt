cmake_minimum_required(VERSION 3.10)
project(Pathtracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMake policy CMP0076 to NEW
if (POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

add_executable(${PROJECT_NAME})

add_subdirectory(src)
