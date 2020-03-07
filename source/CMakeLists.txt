cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv)

# find OpenCV
find_package(OpenCV REQUIRED)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)

# load dlcv library
link_directories(/usr/local/lib)

#################  its  ######################
set(EXECUTABLE_OUTPUT_PATH bin/dlcv)
# install utils exec
add_executable(its its.cc)
install(TARGETS its DESTINATION bin/dlcv)

# link opencv and utils library
target_link_libraries(its
    dlcv_core
    dlcv_its
    curl
    ${OpenCV_LIBS})

# set utils C++20
set_property(TARGET its PROPERTY CXX_STANDARD 20)
#################  its  ######################

#################  vc  ########################
set(EXECUTABLE_OUTPUT_PATH bin/dlcv)
# install utils exec
add_executable(vc vc.cc)
install(TARGETS vc DESTINATION bin/dlcv)

# link opencv and utils library
target_link_libraries(vc
    dlcv_core
    dlcv_vc
    curl
    ${OpenCV_LIBS})

# set utils C++20
set_property(TARGET vc PROPERTY CXX_STANDARD 20)
#################  vc  ########################

#################  ocr  #######################
set(EXECUTABLE_OUTPUT_PATH bin/dlcv)
# install utils exec
add_executable(ocr ocr.cc)
install(TARGETS ocr DESTINATION bin/dlcv)

# link opencv and utils library
target_link_libraries(ocr
    dlcv_core
    dlcv_ocr
    curl
    ${OpenCV_LIBS})

# set utils C++20
set_property(TARGET ocr PROPERTY CXX_STANDARD 20)
#################  ocr  #######################
