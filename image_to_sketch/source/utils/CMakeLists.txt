cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_its)

# find OpenCV
find_package(OpenCV REQUIRED)

# load opencv include
include_directories(/usr/local/include/opencv4/)
# load utils include
include_directories(/usr/local/include/dlcv/)

# make utils shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS_UTILS
    dir.cc
    download.cc
    dlcv_its.cc)
add_library(${PROJECT_NAME} SHARED ${ITS_UTILS})
# install library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib64/dlcv)
else()
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib/dlcv)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# install include
set(INCLUDE
    ../../include/dir.hpp
    ../../include/download.hpp
    ../../include/process.hpp)
install(FILES ${INCLUDE} DESTINATION include/dlcv)

# link opencv library
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


