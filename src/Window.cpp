#include "Window.h"
#include <iostream>

Window::Window(const char *projectName, int width, int height)
{
    scrWidth = width;
    scrHeight = height;
    lastX = width / 2.0f;
    lastY = height / 2.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, projectName, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return;
    }

    // store 'this' so static callbacks can find the instance
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


Window::~Window()
{
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}


float Window::aspectRatio() const
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    if (h == 0) h = 1;
    return (float)w / (float)h;
}


void Window::framebufferSizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}


void Window::mouseCallback(GLFWwindow *w, double xpos, double ypos)
{
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (self) self->onMouseMove(xpos, ypos);
}


void Window::scrollCallback(GLFWwindow *w, double, double yoffset)
{
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (self) self->onScroll(yoffset);
}


void Window::onMouseMove(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void Window::onScroll(double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}