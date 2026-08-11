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
    std::vector<VertexBufferElement> elements;
    unsigned int stride;

public:
    VertexBufferLayout()
        : stride(0) {}

    template<typename T>
    void push(unsigned int count)
    {
        static_assert(sizeof(T) == 0,
            "VertexBufferLayout::push - unsupported type "
            "(use float, unsigned int, or unsigned char)");
    }

    void pushFloat(unsigned int count)
    {
        elements.push_back({ GL_FLOAT, count, GL_FALSE });
        stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    void pushUInt(unsigned int count)
    {
        elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    void pushUByte(unsigned int count)
    {
        elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    const std::vector<VertexBufferElement>& getElements() const { return elements; }
    unsigned int getStride() const { return stride; }
};