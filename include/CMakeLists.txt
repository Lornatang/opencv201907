cmake_minimum_required(VERSION 3.12)

# project name
project(dlcv_include)

# install core include
set(core_include
    dir.hpp
    download.hpp
    log.hpp)
install(FILES ${core_include} DESTINATION include/dlcv)

# install utils include
set(its_include
    dlcv_its.hpp)
install(FILES ${its_include} DESTINATION include/dlcv)

# install vc include
set(vc_include
    dlcv_vc.hpp)
install(FILES ${vc_include} DESTINATION include/dlcv)

# install ocr include
set(ocr_include
    dlcv_ocr.hpp)
install(FILES ${ocr_include} DESTINATION include/dlcv)