cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_exec)

# find OpenCV
find_package(OpenCV REQUIRED)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)

# load dlcv library
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  link_directories(/usr/local/lib)
  link_directories(/usr/local/lib64)
else ()
  link_directories(/usr/local/lib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

################# utils  ######################
set(EXECUTABLE_OUTPUT_PATH bin/dlcv)
# install utils exec
add_executable(its its.cc)
install(TARGETS its DESTINATION bin/dlcv)

# link opencv and utils library
target_link_libraries(its ${OpenCV_LIBS} dlcv_its curl)

# set utils C++17
set_property(TARGET its PROPERTY CXX_STANDARD 17)
################# utils  ######################

#################  vc  ########################
set(EXECUTABLE_OUTPUT_PATH bin/dlcv)
# install utils exec
add_executable(vc vc.cc)
install(TARGETS vc DESTINATION bin/dlcv)

# link opencv and utils library
target_link_libraries(vc ${OpenCV_LIBS} dlcv_vc curl)

# set utils C++17
set_property(TARGET vc PROPERTY CXX_STANDARD 17)
#################  vc  ########################
