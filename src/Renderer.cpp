
#include "Renderer.h"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer::draw(const VertexArray& va, const Shader& shader, unsigned int vertexCount, GLenum mode) const
{
    shader.bind();
    va.bind();
    glDrawArrays(mode,  0, vertexCount);
}


void Renderer::clear() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::setDepthTest(bool turnOn) const
{
    if (turnOn)
    {
        glEnable(GL_DEPTH_TEST);
    } else
    {
        glDisable(GL_DEPTH_TEST);
    }
}
