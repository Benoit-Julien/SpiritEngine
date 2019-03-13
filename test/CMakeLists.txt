cmake_minimum_required(VERSION 3.8)
project(ComputerGraphics1)

set(CMAKE_CXX_STANDARD 17)

include_directories(../src)

set(SOURCES
        main.cpp
        )

add_executable(opengl ${SOURCES})
add_dependencies(opengl SpiritEngine)
target_link_libraries(opengl SpiritEngine)
target_compile_definitions(opengl PRIVATE
        ${CUSTOM_DEFINITIONS}
        MATERIALS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Materials/"
        SHADERS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Shaders/"
        MODELS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Models/")

add_test(NAME opengl COMMAND ./opengl)