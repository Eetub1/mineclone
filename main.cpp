#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include "shader.hpp"
#include "camera.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window initialization
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mineclone", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD, which manages function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    Shader ourShader = Shader("assets/block.vs", "assets/block.fs");

    // Vertices for a single cube
    float vertices[] = {
        // position            // color (back — red)
        -0.5f, -0.5f, -0.5f,   0.8f, 0.2f, 0.2f,
         0.5f, -0.5f, -0.5f,   0.8f, 0.2f, 0.2f,
         0.5f,  0.5f, -0.5f,   0.8f, 0.2f, 0.2f,
         0.5f,  0.5f, -0.5f,   0.8f, 0.2f, 0.2f,
        -0.5f,  0.5f, -0.5f,   0.8f, 0.2f, 0.2f,
        -0.5f, -0.5f, -0.5f,   0.8f, 0.2f, 0.2f,

        // front — green
        -0.5f, -0.5f,  0.5f,   0.2f, 0.7f, 0.3f,
         0.5f, -0.5f,  0.5f,   0.2f, 0.7f, 0.3f,
         0.5f,  0.5f,  0.5f,   0.2f, 0.7f, 0.3f,
         0.5f,  0.5f,  0.5f,   0.2f, 0.7f, 0.3f,
        -0.5f,  0.5f,  0.5f,   0.2f, 0.7f, 0.3f,
        -0.5f, -0.5f,  0.5f,   0.2f, 0.7f, 0.3f,

        // left — blue
        -0.5f,  0.5f,  0.5f,   0.2f, 0.4f, 0.8f,
        -0.5f,  0.5f, -0.5f,   0.2f, 0.4f, 0.8f,
        -0.5f, -0.5f, -0.5f,   0.2f, 0.4f, 0.8f,
        -0.5f, -0.5f, -0.5f,   0.2f, 0.4f, 0.8f,
        -0.5f, -0.5f,  0.5f,   0.2f, 0.4f, 0.8f,
        -0.5f,  0.5f,  0.5f,   0.2f, 0.4f, 0.8f,

        // right — yellow
         0.5f,  0.5f,  0.5f,   0.9f, 0.8f, 0.2f,
         0.5f,  0.5f, -0.5f,   0.9f, 0.8f, 0.2f,
         0.5f, -0.5f, -0.5f,   0.9f, 0.8f, 0.2f,
         0.5f, -0.5f, -0.5f,   0.9f, 0.8f, 0.2f,
         0.5f, -0.5f,  0.5f,   0.9f, 0.8f, 0.2f,
         0.5f,  0.5f,  0.5f,   0.9f, 0.8f, 0.2f,

        // bottom — purple
        -0.5f, -0.5f, -0.5f,   0.6f, 0.3f, 0.7f,
         0.5f, -0.5f, -0.5f,   0.6f, 0.3f, 0.7f,
         0.5f, -0.5f,  0.5f,   0.6f, 0.3f, 0.7f,
         0.5f, -0.5f,  0.5f,   0.6f, 0.3f, 0.7f,
        -0.5f, -0.5f,  0.5f,   0.6f, 0.3f, 0.7f,
        -0.5f, -0.5f, -0.5f,   0.6f, 0.3f, 0.7f,

        // top — orange
        -0.5f,  0.5f, -0.5f,   0.9f, 0.5f, 0.2f,
         0.5f,  0.5f, -0.5f,   0.9f, 0.5f, 0.2f,
         0.5f,  0.5f,  0.5f,   0.9f, 0.5f, 0.2f,
         0.5f,  0.5f,  0.5f,   0.9f, 0.5f, 0.2f,
        -0.5f,  0.5f,  0.5f,   0.9f, 0.5f, 0.2f,
        -0.5f,  0.5f, -0.5f,   0.9f, 0.5f, 0.2f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);


    while(!glfwWindowShouldClose(window))
    {
        // Calculate time stuff
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600.0f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        //glm::mat4 model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // Draw cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}