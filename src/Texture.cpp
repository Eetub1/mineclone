#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Stb_image.h"
#include "Texture.h"

Texture::Texture(const std::string &filepath)
    : id(0), filepath(filepath), localBuffer(nullptr), width(0), height(0), BPP(0)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(filepath.c_str(), &width, &height, &BPP, 0);
    if (localBuffer)
    {
        GLenum format = GL_RGB;
        if (BPP == 1)      format = GL_RED;
        else if (BPP == 3) format = GL_RGB;
        else if (BPP == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(localBuffer);
}


Texture::~Texture()
{
    glDeleteTextures(1, &id);
}


void Texture::bind(unsigned int slot /*= 0*/) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}


void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
