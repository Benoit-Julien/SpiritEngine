cmake_minimum_required(VERSION 3.8)
project(DeferredShading)

set(CMAKE_CXX_STANDARD 17)

include_directories(../src)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} SpiritEngine)
target_link_libraries(${PROJECT_NAME} SpiritEngine)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        ${CUSTOM_DEFINITIONS}
        MATERIALS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Materials/"
        TEXTURES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Textures/"
        SHADERS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Shaders/"
        MODELS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Models/")
