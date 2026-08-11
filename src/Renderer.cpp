
#include "Renderer.h"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.use(); // Remember, this uses the older version of Shader.h, not the cherno version!
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}
