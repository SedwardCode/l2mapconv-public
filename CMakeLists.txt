cmake_minimum_required(VERSION 3.22)
project(l2mapconv)

# Install directory
set(INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR})

# Compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
    CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
    CMAKE_CXX_COMPILER_ID STREQUAL AppleClang)

    set(TARGET_COMPILE_OPTIONS -Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)

    set(TARGET_COMPILE_OPTIONS /Wall /WX)
endif()

# Modules
add_subdirectory(libs)

add_subdirectory(utils)
add_subdirectory(geometry)
add_subdirectory(unreal)
add_subdirectory(rendering)
add_subdirectory(geodata)

add_subdirectory(application)
