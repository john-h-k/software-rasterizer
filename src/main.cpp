#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>

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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pixels", NULL, NULL);
    if (!window)
    {
        std::cerr << "GLFW window could not initialise" << std::endl;
        glfwTerminate();
        return -1;
    }

    sr::SoftwareRasteriser softwareRasteriser(SCREEN_WIDTH, SCREEN_HEIGHT);
    softwareRasteriser.colour(0xff, 0xff, 0x00);

    uint64_t framecount = 0;
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, softwareRasteriser.getBuffer());
        softwareRasteriser.colour(0xff, 0xff * (framecount % 2), 0x00);
        glfwSwapBuffers(window);
        framecount++;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
