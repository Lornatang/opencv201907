cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_utils)

# find OpenCV
find_package(OpenCV REQUIRED)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)

################# its  ######################
# make its shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS_SOURCE dlcv_its.cc)
add_library(dlcv_its SHARED ${ITS_SOURCE})
# install its library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS dlcv_its LIBRARY DESTINATION lib64)
else()
  install(TARGETS dlcv_its LIBRARY DESTINATION lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# link its library
target_link_libraries(dlcv_its ${OpenCV_LIBS} core)
# set its C++17
set_property(TARGET dlcv_its PROPERTY CXX_STANDARD 17)
################# its  ######################

