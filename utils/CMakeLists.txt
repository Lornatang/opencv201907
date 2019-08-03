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

################# utils  ######################
# make utils shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS_SOURCE dlcv_its.cc)
add_library(dlcv_its SHARED ${ITS_SOURCE})
# install utils library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS dlcv_its LIBRARY DESTINATION lib64)
else()
  install(TARGETS dlcv_its LIBRARY DESTINATION lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# link utils library
target_link_libraries(dlcv_its ${OpenCV_LIBS} core curl)
# set utils C++17
set_property(TARGET dlcv_its PROPERTY CXX_STANDARD 17)
################# utils  ######################

#################  vc  ########################
# make utils shared library
set(LIBRARY_OUTPUT_PATH lib)
set(VC_SOURCE dlcv_vc.cc)
add_library(dlcv_vc SHARED ${VC_SOURCE})
# install utils library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS dlcv_vc LIBRARY DESTINATION lib64)
else()
  install(TARGETS dlcv_vc LIBRARY DESTINATION lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# link utils library
target_link_libraries(dlcv_vc ${OpenCV_LIBS} core curl)
# set utils C++17
set_property(TARGET dlcv_vc PROPERTY CXX_STANDARD 17)
#################  vc  ########################
