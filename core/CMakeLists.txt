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
add_library(core SHARED ${CORE_SOURCE})
# install library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS core LIBRARY DESTINATION lib64)
else()
  install(TARGETS core LIBRARY DESTINATION lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# link opencv library
target_link_libraries(core ${OpenCV_LIBS})

# set C++17
set_property(TARGET core PROPERTY CXX_STANDARD 17)


