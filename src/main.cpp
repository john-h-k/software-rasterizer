#include <GLFW/glfw3.h>
#include <iostream>

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Pixels", NULL, NULL);
    if (!window)
    {
        std::cerr << "GLFW window could not initialise" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
