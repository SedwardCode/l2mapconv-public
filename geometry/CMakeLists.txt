cmake_minimum_required(VERSION 3.22)
project(geometry)

add_library(${PROJECT_NAME}
    src/Box.cpp
    src/Frustum.cpp
    src/Sphere.cpp
    src/Triangle.cpp
    src/Transformation.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE utils

    PRIVATE glm
)

# Compiler settings
set_target_properties(${PROJECT_NAME} PROPERTIES ${TARGET_PROPERTIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})
