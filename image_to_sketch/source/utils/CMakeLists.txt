cmake_minimum_required(VERSION 3.12)

project(its)

# find OpenCV
find_package(OpenCV REQUIRED)

# load opencv include
include_directories(/usr/local/include/opencv4/)

# make its shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS
    dir.cc
    download.cc
    process.cc)
add_library(its SHARED ${ITS})
# install library
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

# install include
set(INCLUDE
    ../../include/dir.hpp
    ../../include/download.hpp
    ../../include/process.hpp)
install(FILES ${INCLUDE} DESTINATION include)

# load library
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

