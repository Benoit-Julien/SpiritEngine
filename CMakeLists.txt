cmake_minimum_required(VERSION 3.8)
project(GLEngine)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL 4.0 REQUIRED)
find_package(glfw3 3.2 REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_directories(${OPENGL_gl_LIBRARY})
endif ()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
endif ()

find_package(glm REQUIRED)
if (GLM_FOUND)
    include_directories(${glm_DIR})
endif ()

add_subdirectory(imgui)
add_subdirectory(soil)

set(LIBRARIES
        imgui
        soil
        glfw
        ${GLEW_LIBRARIES}
        glm
        ${OPENGL_gl_LIBRARY}
        ${ASSIMP_LIBRARIES}
        )

include_directories(rapidjson)
include_directories(imgui)
include_directories(soil/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
        src/MyGlWindow.cpp
        src/Viewer.cpp
        src/Scene.cpp
        src/Material.cpp
        src/Shader.cpp
        src/Objects/Object.cpp
        src/Objects/Movable.cpp
        src/Objects/Drawable.cpp
        src/Objects/Light.cpp
        src/Objects/TriangleObject.cpp
        src/Objects/Model3D.cpp
        src/Objects/Primitives/Cube.cpp
        src/Objects/Primitives/Plane.cpp
        src/Objects/Primitives/Sphere.cpp
        )
        #src/Objects/LineSegment.cpp
        #src/Objects/Primitives/WireCube.cpp
        #src/Objects/Primitives/CheckeredFloor.cpp
        #)

add_library(GLEngine STATIC ${SOURCES})
target_link_libraries(GLEngine ${LIBRARIES})
add_dependencies(GLEngine imgui soil)