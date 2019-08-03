cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)

# load dlcv library
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  link_directories(/usr/local/lib)
  link_directories(/usr/local/lib/dlcv)
  link_directories(/usr/local/lib64)
  link_directories(/usr/local/lib64/dlcv)
else ()
  link_directories(/usr/local/lib)
  link_directories(/usr/local/lib/dlcv)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# install exec
set(EXECUTABLE_OUTPUT_PATH bin)
add_executable(${PROJECT_NAME} main.cc)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# link opencv and its library
target_link_libraries(${PROJECT_NAME} dlcv_its ${OpenCV_LIBS})

# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
