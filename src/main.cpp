#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>
#include <vector>

#include "SoftwareRasterizer.h"

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

    sr::SoftwareRasterizer SoftwareRasterizer(SCREEN_WIDTH, SCREEN_HEIGHT);
    SoftwareRasterizer.colour(0xff, 0xff, 0x00);

    int v1[3] = {SCREEN_WIDTH / 2, 0, 1};
    int v2[3] = {(SCREEN_WIDTH * 3)/4, SCREEN_HEIGHT, 1};
    int v3[3] = {0, SCREEN_HEIGHT, 1};
    std::vector<int*> vertices = {v1, v2, v3};
    int f1[3] = {0,1,2};
    std::vector<int*> faces = {f1};
    uint8_t tr[3] = {0xff, 0, 0};
    uint8_t tg[3] = {0, 0xff, 0};
    uint8_t tb[3] = {0, 0, 0xff};
    std::vector<uint8_t*> vertex_colours = {tr, tg, tb};


    SoftwareRasterizer.load(vertices.data(), faces.size(), faces.data(), vertex_colours.data());

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SoftwareRasterizer.render();
        glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, SoftwareRasterizer.getBuffer());
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
