cmake_minimum_required(VERSION 3.22)
project(l2mapconv)

set(INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INSTALL_DIR})

add_subdirectory(libs)

add_subdirectory(utils)
add_subdirectory(geometry)
add_subdirectory(unreal)
add_subdirectory(rendering)
add_subdirectory(geodata)

add_subdirectory(application)
