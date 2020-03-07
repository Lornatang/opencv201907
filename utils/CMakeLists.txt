cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_utils)

# find OpenCV
find_package(OpenCV REQUIRED)

# load system include
include_directories(/usr/local/include)
# load opencv include
include_directories(/usr/local/include/opencv4)
# load dlcv include
include_directories(/usr/local/include/dlcv)
# load leptonica include
include_directories(/usr/local/include/leptonica)
# load tesseract include
include_directories(/usr/local/include/tesseract)

################# its  ######################
# make utils shared library
set(LIBRARY_OUTPUT_PATH lib)
set(ITS_SOURCE dlcv_its.cc)
add_library(dlcv_its SHARED ${ITS_SOURCE})
# install utils library
# must check system os
install(TARGETS dlcv_its LIBRARY DESTINATION lib)


# link utils library
target_link_libraries(dlcv_its ${OpenCV_LIBS} dlcv_core curl)
# set utils C++20
set_property(TARGET dlcv_its PROPERTY CXX_STANDARD 20)
################# its  ######################

#################  vc  ########################
# make utils shared library
set(LIBRARY_OUTPUT_PATH lib)
set(VC_SOURCE dlcv_vc.cc)
add_library(dlcv_vc SHARED ${VC_SOURCE})
# install utils library
# must check system os
install(TARGETS dlcv_vc LIBRARY DESTINATION lib)

# link utils library
target_link_libraries(dlcv_vc ${OpenCV_LIBS} dlcv_core curl)
# set utils C++20
set_property(TARGET dlcv_vc PROPERTY CXX_STANDARD 20)
#################  vc  ########################

#################  ocr  #######################
# make utils shared library
set(LIBRARY_OUTPUT_PATH lib)
set(OCR_SOURCE dlcv_ocr.cc)
add_library(dlcv_ocr SHARED ${OCR_SOURCE})
# install utils library
# must check system os
install(TARGETS dlcv_ocr LIBRARY DESTINATION lib)
# link utils library
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(dlcv_ocr
      ${OpenCV_LIBS}
      dlcv_core
      curl
      leptonica
      tesseract)
else ()
  target_link_libraries(dlcv_ocr
      ${OpenCV_LIBS}
      dlcv_core
      curl
      /usr/local/lib/libleptonica.1.79.0.dylib
      /usr/local/lib/libtesseract.5.0.0.dylib)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

# set utils C++20
set_property(TARGET dlcv_ocr PROPERTY CXX_STANDARD 20)
#################  ocr  ######################
