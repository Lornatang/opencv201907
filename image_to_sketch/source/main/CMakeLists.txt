# minnum opencv version
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# project name
project(its)

# find opencv
find_package(OpenCV REQUIRED)

# load opencv include
include_directories(/usr/local/include/opencv4)
# load its include
include_directories(${PROJECT_SOURCE_DIR}/include)

#
set(ITS_SOURCE
    )
add_executable(${PROJECT_NAME}
    source/main.cc
    include/dir.hpp
    include/download.hpp
    include/log.hpp
    include/process.hpp)

# link its library
target_link_libraries(${PROJECT_NAME} ${TORCH_LIBRARIES} ${OpenCV_LIBS})
# set C++17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


 

