cmake_minimum_required(VERSION 3.8)
project(SpiritEngine)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL 4.1 REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_directories(${OPENGL_gl_LIBRARY})
endif ()

add_subdirectory(glad)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/glad/include)

set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
include_directories(glfw/include)

set(BUILD_SHARED_LIBS ON)
set(BUILD_STATIC_LIBS OFF)
set(GLM_TEST_ENABLE_CXX_17 ON)
set(GLM_TEST_ENABLE OFF)
add_subdirectory(glm)
include_directories(glm)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)
include_directories(assimp/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/assimp/include)

set(OPENGL_LOADER_GLAD ON)
add_subdirectory(imgui)
include_directories(imgui)

set(LIBRARIES
        ${OPENGL_gl_LIBRARY}
        glad
        glfw
        glm_shared
        imgui
        assimp
        )

include_directories(rapidjson/include)
include_directories(stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
        src/MyGlWindow.cpp
        src/Viewer.cpp
        src/Scene.cpp
        src/Material.cpp
        src/Texture.cpp
        src/FileLoader/MaterialsFileLoader.cpp
        src/FileLoader/TexturesFileLoader.cpp
        src/Shader/ShaderProgram.cpp
        src/Shader/AShader.cpp
        src/Shader/VertexShader.cpp
        src/Shader/FragmentShader.cpp
        src/Objects/Object.cpp
        src/Objects/Movable.cpp
        src/Objects/Drawable.cpp
        src/Objects/Lights/Light.cpp
        src/Objects/Lights/SpotLight.cpp
        src/Objects/TriangleObject.cpp
        src/Objects/Mesh.cpp
        src/Objects/Primitives/Cube.cpp
        src/Objects/Primitives/Plane.cpp
        src/Objects/Primitives/Sphere.cpp
        )
        #src/Objects/LineSegment.cpp
        #src/Objects/Primitives/WireCube.cpp
        #src/Objects/Primitives/CheckeredFloor.cpp
        #)

add_library(SpiritEngine SHARED ${SOURCES})
target_link_libraries(SpiritEngine ${LIBRARIES})
add_dependencies(SpiritEngine glad glfw glm imgui assimp)

set(CUSTOM_DEFINITIONS)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CUSTOM_DEFINITIONS ${CUSTOM_DEFINITIONS} _DEBUG)
endif()

target_compile_definitions(SpiritEngine PRIVATE ${CUSTOM_DEFINITIONS})

add_subdirectory(test)