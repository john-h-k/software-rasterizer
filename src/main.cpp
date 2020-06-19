#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>

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
    uint8_t* screenBuffer = new uint8_t[SCREEN_WIDTH * SCREEN_HEIGHT * 3];
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            screenBuffer[y * SCREEN_WIDTH * 3 + x * 3] = 0x25;
            screenBuffer[y * SCREEN_WIDTH * 3 + x * 3 + 1] = 0x34;
            screenBuffer[y * SCREEN_WIDTH * 3 + x * 3 + 2] = 0xff;
        }
    }
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawPixels(640, 480, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
