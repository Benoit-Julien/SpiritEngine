cmake_minimum_required(VERSION 3.8)
project(SpiritEngine)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(glad)
include_directories(glad/include)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
add_subdirectory(glfw)
include_directories(glfw/include)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLM_TEST_ENABLE_CXX_17 ON CACHE BOOL "")
set(GLM_TEST_ENABLE OFF CACHE BOOL "")
add_subdirectory(glm)
include_directories(glm)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
add_subdirectory(assimp)
include_directories(assimp/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/assimp/include)

set(OPENGL_LOADER_GLAD ON CACHE BOOL "")
add_subdirectory(imgui)
include_directories(imgui)

set(LIBRARIES
        glad
        glfw
        glm_static
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
        src/utils.cpp
        src/FBO/FboManager.cpp
        src/FBO/TextureManager.cpp
        src/PostProcessing.cpp
        src/Texture/Texture.cpp
        src/Texture/Texture2D.cpp
        src/Texture/SkyBoxTexture.cpp
        src/FileLoader/MaterialsFileLoader.cpp
        src/FileLoader/TexturesFileLoader.cpp
        src/FileLoader/ShadersFileLoader.cpp
        src/Shader/ShaderProgram.cpp
        src/Shader/AShader.cpp
        src/Shader/VertexShader.cpp
        src/Shader/FragmentShader.cpp
        src/Objects/Object.cpp
        src/Objects/Movable.cpp
        src/Objects/Drawable.cpp
        src/Objects/CullFace.cpp
        src/Objects/Lights/Light.cpp
        src/Objects/Lights/SpotLight.cpp
        src/Objects/TriangleObject.cpp
        src/Objects/Mesh.cpp
        src/Objects/Primitives/Cube.cpp
        src/Objects/Primitives/Plane.cpp
        src/Objects/Primitives/Sphere.cpp
        src/Objects/Primitives/SkyBox.cpp
        )
#src/Objects/LineSegment.cpp
#src/Objects/Primitives/WireCube.cpp
#src/Objects/Primitives/CheckeredFloor.cpp
#)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG")

if (CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif ()

add_library(SpiritEngine STATIC ${SOURCES})
target_link_libraries(SpiritEngine ${LIBRARIES})
add_dependencies(SpiritEngine glad glfw glm_static imgui assimp)

if (WIN32)
    set(CUSTOM_DEFINITIONS ${CUSTOM_DEFINITIONS} WINDOWS)
endif ()

target_compile_definitions(SpiritEngine PRIVATE ${CUSTOM_DEFINITIONS})

add_subdirectory(test)