cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_its)

# find OpenCV
find_package(OpenCV REQUIRED)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)

# make its shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS_UTILS
    dir.cc
    dlcv_its.cc
    download.cc)
add_library(${PROJECT_NAME} SHARED ${ITS_UTILS})
# install library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib64)
else()
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# link opencv library
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


