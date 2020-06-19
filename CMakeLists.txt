cmake_minimum_required(VERSION 3.15)
project(SoftwareRasteriser)

set(CMAKE_CXX_STANDARD 20)

add_executable(SoftwareRasteriser src/main.cpp)

if (WIN32)
    target_include_directories(SoftwareRasteriser PRIVATE ./external/include)
    target_link_directories(SoftwareRasteriser PRIVATE ./external/libs)
endif()

find_library(GLFW_LIB glfw)
target_link_libraries(SoftwareRasteriser PRIVATE "${GLFW_LIB}")