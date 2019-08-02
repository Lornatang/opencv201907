cmake_minimum_required(VERSION 3.12)

# project name
project(sketch)

# set build mode
set(CMAKE_BUILD_TYPE "Release")
# set install path
set(CMAKE_INSTALL_PREFIX "/usr/local")

add_subdirectory(source/utils)
add_subdirectory(source/main)