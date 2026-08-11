#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void draw(const VertexArray& va, const Shader& shader, unsigned int vertexCount, GLenum mode = GL_TRIANGLES) const;

    void clear() const;
    void setDepthTest(bool turnOn) const;

};