#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>
#include <vector>

#include "SoftwareRasteriser.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << " - code: " << error << std::endl;
}

int main() {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        // failed
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pixels", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "GLFW window could not initialise" << std::endl;
        glfwTerminate();
        return -1;
    }

    sr::SoftwareRasteriser softwareRasteriser(SCREEN_WIDTH, SCREEN_HEIGHT);
    softwareRasteriser.colour(0xff, 0xff, 0x00);

    sr::Vec3 v1 = {SCREEN_WIDTH / 2, 0, 1};
    sr::Vec3 v2 = {SCREEN_WIDTH, SCREEN_HEIGHT, 1};
    sr::Vec3 v3 = {0, SCREEN_HEIGHT, 1};
    std::vector<sr::Vec3> vertices = {v1, v2, v3};
    sr::Vec3 f1 = {0, 1, 2};
    std::vector<sr::Vec3> faces = {f1};
    sr::Colour tr = {0xff, 0, 0};
    sr::Colour tg = {0, 0xff, 0};
    sr::Colour tb = {0, 0, 0xff};
    std::vector<sr::Colour> vertex_colours = {tr, tg, tb};


    softwareRasteriser.load(vertices.data(), faces.size(), faces.data(), vertex_colours.data());

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        softwareRasteriser.render();
        glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, softwareRasteriser.getBuffer());
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
