cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv)

# set build mode
set(CMAKE_BUILD_TYPE "Release")
# set install path
set(CMAKE_INSTALL_PREFIX "/usr/local")

# add include to path
add_subdirectory(include)
# add Image to Sketch to path
add_subdirectory(its)