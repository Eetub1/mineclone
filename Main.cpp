#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "Vertices.h" // vertex data

void processInput(Window &win, Camera &camera, float deltaTime);


static float lastFrame = 0.0f; // Time of last frame


int main()
{
    Window window = Window("Mineclone", 800, 600);
    if (!window.isValid()) return -1;

    Shader ourShader = Shader("assets/block.vert", "assets/block.frag");

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // Texture positions
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    std::vector<glm::vec3> cubes;
    // create pyramid shape
    for (int level = 0; level < 3; level++)
    {
        int half = 2 - level;
        for (int x = -half; x <= half; x++)
            for (int z = -half; z <= half; z++)
                cubes.push_back(glm::vec3(x, level, z));
    }

    // hill
    /*for (int x = -8; x <= 8; x++)
        for (int z = -8; z <= 8; z++)
        {
            int h = (int)(2.0f * sinf(x * 0.4f) + 2.0f * cosf(z * 0.4f));
            for (int y = -3; y <= h; y++)
                cubes.push_back(glm::vec3(x, y, z));
        }*/

    Texture dirt = Texture("assets/dirt.jpg");
    dirt.bind();


    while(!window.shouldClose())
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, window.getCamera(), deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Fixes an issue where cubes don't look like cubes
        int fbW, fbH;
        glfwGetFramebufferSize(window.handle(), &fbW, &fbH);
        float aspect = static_cast<float>(fbW) / static_cast<float>(fbH);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(window.getCamera().Zoom), aspect, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = window.getCamera().GetViewMatrix();
        ourShader.setMat4("view", view);

        glBindVertexArray(VAO);

        for (auto cube : cubes)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window.handle());
        glfwPollEvents();
    }

    return 0;
}


void processInput(Window &win, Camera &camera, float deltaTime)
{
    GLFWwindow *window = win.handle();

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