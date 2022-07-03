cmake_minimum_required(VERSION 3.22)
project(unreal)

add_library(${PROJECT_NAME}
    src/pch.cpp
    src/Debug.cpp

    # Archive loading
    src/PackageLoader.cpp
    src/ArchiveLoader.cpp
    src/Decryptor.cpp
    src/Archive.cpp

    # Property extraction
    src/PropertyExtractor.cpp
    src/ArrayIndex.cpp
    src/Property.cpp

    # Object loading
    src/ObjectLoader.cpp
    src/Object.cpp

    # Objects
    src/Primitives.cpp
    src/Level.cpp
    src/BSP.cpp
    src/StaticMesh.cpp
    src/Material.cpp
    src/Actor.cpp
    src/Terrain.cpp
    src/MaterialDeserializer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

target_link_libraries(${PROJECT_NAME}
    PUBLIC utils
)

# Compiler settings
set_target_properties(${PROJECT_NAME} PROPERTIES ${TARGET_PROPERTIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})
