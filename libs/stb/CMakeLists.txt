cmake_minimum_required(VERSION 3.22)
project(stb)

add_library(${PROJECT_NAME}
    src/stb_image.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
