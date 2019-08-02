cmake_minimum_required(VERSION 3.12)

# project name
project(its)

# find OpenCV
find_package(OpenCV REQUIRED)

# load opencv include
include_directories(/usr/local/include/opencv4/)

# make its shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS_UTILS
    dir.cc
    download.cc
    process.cc)
add_library(its SHARED ${ITS_UTILS})
# install library
# must check system os
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  install(TARGETS its LIBRARY DESTINATION lib64)
else()
  install(TARGETS its LIBRARY DESTINATION lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# install include
set(INCLUDE
    ../../include/dir.hpp
    ../../include/download.hpp
    ../../include/process.hpp)
install(FILES ${INCLUDE} DESTINATION include)

# link opencv library
target_link_libraries(its ${OpenCV_LIBS})

# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


