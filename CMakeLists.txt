cmake_minimum_required(VERSION 3.15)
project(SoftwareRasteriser__)

set(CMAKE_CXX_STANDARD 20)

add_executable(SoftwareRasteriser__ src/main.cpp)

find_library(GLFW_LIB glfw)
target_link_libraries(SoftwareRasteriser__ PRIVATE "${GLFW_LIB}")