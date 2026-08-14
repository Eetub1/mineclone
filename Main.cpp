#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"
#include "Shader.h"
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
    blockLayout.pushFloat(3);
    blockLayout.pushFloat(2);
    blockVao.addBuffer(blockVbo, blockLayout);


    Shader crosshairShader = Shader("assets/crosshair.vert", "assets/crosshair.frag");
    auto crosshairVao = VertexArray();
    auto crosshairVbo = VertexBuffer(crosshairVertices, sizeof(crosshairVertices));
    auto crosshairLayout = VertexBufferLayout();
    crosshairLayout.pushFloat(2);
    crosshairVao.addBuffer(crosshairVbo, crosshairLayout);


    Shader lightsourceShader = Shader("assets/lightsource.vert", "assets/lightsource.frag");
    auto lightsourceVao = VertexArray();
    lightsourceVao.addBuffer(blockVbo, blockLayout); // reusing the block's vbo and layout


    std::vector<glm::vec3> cubes;
    for (int x = 0; x < 25; x++)
    {
        for (int z = 0; z < 25; z++)
        {
            cubes.push_back(glm::vec3(x, -3, z));
        }
    }

    Texture dirt = Texture("assets/dirt.jpg");
    dirt.bind();

    Renderer renderer;
    renderer.setDepthTest(true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while(!window.shouldClose())
    {
        // This could also be moved somewhere else
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Camera &camera = window.getCamera();

        processInput(window, camera, deltaTime);

        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Camera: %.1f %.1f %.1f", camera.Position.x, camera.Position.y, camera.Position.z);
        ImGui::Text("Blocks: %zu", cubes.size());
        ImGui::End();

        float aspect = window.aspectRatio();

        blockShader.bind();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
        blockShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        blockShader.setMat4("view", view);


        for (auto &cube : cubes)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            blockShader.setMat4("model", model);
            renderer.draw(blockVao, blockShader, 36);
        }

        renderer.setDepthTest(false);

        // Crosshair
        crosshairShader.bind();
        crosshairShader.setFloat("aspect", aspect);
        renderer.draw(crosshairVao, crosshairShader, 4, GL_LINES);

        renderer.setDepthTest(true);
        
        // Lightsource
        lightsourceShader.bind();
        lightsourceShader.setMat4("projection", projection);
        lightsourceShader.setMat4("view", view);
        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
        lightsourceShader.setMat4("model", model);
        renderer.draw(lightsourceVao, lightsourceShader, 36);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.handle());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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