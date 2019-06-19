cmake_minimum_required(VERSION 3.8)
project(Glad)

set(CMAKE_CXX_STANDARD 11)
set(SOURCES src/glad.c)

include_directories(include)
add_library(glad STATIC ${SOURCES})
