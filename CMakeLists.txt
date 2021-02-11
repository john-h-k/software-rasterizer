cmake_minimum_required(VERSION 3.15)
project(SoftwareRasterizer)

set(CMAKE_CXX_STANDARD 20)

add_executable(SoftwareRasterizer src/main.cpp src/SoftwareRasterizer.cpp "inc/SoftwareRasterizer.h" "inc/RasterizerDesc.h")

target_include_directories(SoftwareRasterizer PRIVATE "./inc")

if (WIN32)
    target_include_directories(SoftwareRasterizer PRIVATE "./external/include")
    target_link_directories(SoftwareRasterizer PRIVATE "./external/libs")
    find_library(GLFW_LIB glfw3 "./external/libs/")
    find_package(OpenGL REQUIRED)
    target_link_libraries(SoftwareRasterizer PRIVATE "${GLFW_LIB}" OpenGL::GL)
endif()

if (UNIX)
    find_library(GLFW_LIB glfw REQUIRED)
    find_library(OPENGL_LIB GL REQUIRED)
    target_link_libraries(SoftwareRasterizer PRIVATE "${GLFW_LIB}" "${OPENGL_LIB}")
endif()
