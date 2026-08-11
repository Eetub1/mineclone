#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


//#include "Shader.h" // This is theCherno's version

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "ShaderOLDER.h"
#include "Window.h"
#include "Texture.h"
#include "Vertices.h" // block and crosshair vertex data

void processInput(Window &win, Camera &camera, float deltaTime);

static float lastFrame = 0.0f; // Time of last frame


int main()
{
    Window window = Window("Mineclone", 800, 600);
    if (!window.isValid()) return -1;

    Shader blockShader = Shader("assets/block.vert", "assets/block.frag");
    auto blockVao = VertexArray();
    auto blockVbo = VertexBuffer(vertices, sizeof(vertices));
    auto blockLayout = VertexBufferLayout();
    blockLayout.pushFloat(3);
    blockLayout.pushFloat(2);
    blockVao.addBuffer(blockVbo, blockLayout);

    Shader crosshairShader = Shader("assets/crosshair.vert", "assets/crosshair.frag");
    auto crosshairVao = VertexArray();
    auto crosshairVbo = VertexBuffer(crosshairVertices, sizeof(crosshairVertices));
    auto crosshairLayout = VertexBufferLayout();
    crosshairLayout.pushFloat(2);
    crosshairVao.addBuffer(crosshairVbo, crosshairLayout);


    std::vector<glm::vec3> cubes;
    // create pyramid shape
    for (int level = 0; level < 3; level++)
    {
        int half = 2 - level;
        for (int x = -half; x <= half; x++)
            for (int z = -half; z <= half; z++)
                cubes.push_back(glm::vec3(x, level, z));
    }

    Texture dirt = Texture("assets/dirt.jpg");
    dirt.bind();

    glEnable(GL_DEPTH_TEST);
    
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

        blockShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(window.getCamera().Zoom), aspect, 0.1f, 100.0f);
        blockShader.setMat4("projection", projection);

        glm::mat4 view = window.getCamera().GetViewMatrix();
        blockShader.setMat4("view", view);

        blockVao.bind();

        for (auto &cube : cubes)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            blockShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Crosshair shader stuff
        glDisable(GL_DEPTH_TEST); // Crosshair can't be see through
        crosshairShader.use();
        crosshairShader.setFloat("aspect", aspect);
        crosshairVao.bind();
        glDrawArrays(GL_LINES, 0, 4);
        glEnable(GL_DEPTH_TEST);

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