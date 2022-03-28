cmake_minimum_required(VERSION 3.22)
project(math)

add_library(${PROJECT_NAME}
    src/Transformation.cpp
    src/Box.cpp
    src/Frustum.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE utils

    PRIVATE glm
)

# Compiler options
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
