cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_core)

# find OpenCV
find_package(OpenCV REQUIRED)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)

# make dlcv core shared library
set(LIBRARY_OUTPUT_PATH lib)
set(CORE_SOURCE
    dir.cc
    download.cc)
add_library(${PROJECT_NAME} SHARED ${CORE_SOURCE})
# install library
# must check system os
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)


# link opencv library
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} curl)

# set C++20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)


