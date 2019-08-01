cmake_minimum_required(VERSION 3.12)

project(libocr)

# find OpenCV
find_package(OpenCV REQUIRED)

# load opencv include
include_directories(/usr/local/include/opencv4/)

# make SHARED library
set(LIBRARY_OUTPUT_PATH lib)
set(ANGLE
    angle.cc
    format.cc
    rotation.cc )
add_library(ocr SHARED ${ANGLE})
# install library to path
install(TARGETS ocr LIBRARY DESTINATION lib)

# install include to path
set(INCLUDE
    ../../include/log.hpp
    ../../include/format.hpp
    ../../include/pretreatment.hpp)
install(FILES ${INCLUDE} DESTINATION include)

# load library
target_link_libraries(ocr ${TORCH_LIBRARIES} ${OpenCV_LIBS})
# set C++17
set_property(TARGET ocr PROPERTY CXX_STANDARD 17)

