cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(VideoCover)

find_package(OpenCV REQUIRED)       # find OpenCV
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(/usr/local/include/)
include_directories(/usr/local/include/opencv4/opencv2)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(VideoCover source/main.cc
        include/detect.hpp
        include/dir.hpp
        include/download.hpp
        include/log.hpp
        include/process.hpp)

target_link_libraries(VideoCover ${TORCH_LIBRARIES} ${OpenCV_LIBS})
set_property(TARGET VideoCover PROPERTY CXX_STANDARD 17)


 

