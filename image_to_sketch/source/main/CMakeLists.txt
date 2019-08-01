cmake_minimum_required(VERSION 3.12)

# project name
project(sketch)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# link its library
link_directories(/usr/local/lib)

# install exec
set(EXECUTABLE_OUTPUT_PATH bin)
add_executable(${PROJECT_NAME} main.cc)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# link library
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} its)
# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
