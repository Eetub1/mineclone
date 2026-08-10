#pragma once

#include <glad/glad.h>
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:         return 4;
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
        : m_Stride(0) {}

    // Generic entry point. Only the types handled below are supported;
    // anything else fails to compile with a clear message.
    template<typename T>
    void push(unsigned int count)
    {
        static_assert(sizeof(T) == 0,
            "VertexBufferLayout::push - unsupported type "
            "(use float, unsigned int, or unsigned char)");
    }

    void pushFloat(unsigned int count)
    {
        m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    void pushUInt(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    void pushUByte(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
    unsigned int getStride() const { return m_Stride; }
};