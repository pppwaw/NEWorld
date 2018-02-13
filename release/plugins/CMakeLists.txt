cmake_minimum_required(VERSION 3.1)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_DIR ./../../src)

project(MainPlugin)

aux_source_directory(${SOURCE_DIR}/main SRC_MAINPLUGIN)
add_library(main MODULE ${SRC_MAINPLUGIN})
target_link_libraries(main GUI nwcore)

SET_TARGET_PROPERTIES(main PROPERTIES PREFIX "")
