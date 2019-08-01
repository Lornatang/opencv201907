cmake_minimum_required(VERSION 3.12)

project(idocr)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load ocr library
include_directories(/usr/local/lib)

# install idocr
set(EXECUTABLE_OUTPUT_PATH bin)
add_executable(idocr main.cc)
install(TARGETS idocr DESTINATION bin)

# load opencv library
target_link_libraries(${PROJECT_NAME} ${TORCH_LIBRARIES} ${OpenCV_LIBS} ocr)
# set C++17
set_property(TARGET idocr PROPERTY CXX_STANDARD 17)
