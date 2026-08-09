#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"

class Window
{
public:
    Window(const char *projectName, int width = 800, int height = 600);
    ~Window();

    bool isValid() const { return window != nullptr; }
    bool shouldClose() const { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents() { glfwPollEvents(); }
    GLFWwindow* handle() const { return window; }

    float aspectRatio() const;
    Camera& getCamera() { return camera; }

private:
    GLFWwindow *window = nullptr;
    int scrWidth, scrHeight;

    Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
    float lastX, lastY;
    bool firstMouse = true;

    static void framebufferSizeCallback(GLFWwindow *w, int width, int height);
    static void mouseCallback(GLFWwindow *w, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *w, double xoffset, double yoffset);

    void onMouseMove(double xpos, double ypos);
    void onScroll(double yoffset);
};