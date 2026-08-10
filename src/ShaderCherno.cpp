#include "ShaderCherno.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string &filepath)
    : m_RendererID(0), m_Filepath(filepath)
{
    ShaderProgramSource source = parseShader(filepath);
    m_RendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    if (m_RendererID)
        glDeleteProgram(m_RendererID);
}

Shader::Shader(Shader &&other) noexcept
    : m_RendererID(other.m_RendererID),
      m_Filepath(std::move(other.m_Filepath)),
      m_UniformLocationCache(std::move(other.m_UniformLocationCache))
{
    other.m_RendererID = 0;
}

Shader& Shader::operator=(Shader &&other) noexcept
{
    if (this != &other)
    {
        if (m_RendererID)
            glDeleteProgram(m_RendererID);

        m_RendererID = other.m_RendererID;
        m_Filepath = std::move(other.m_Filepath);
        m_UniformLocationCache = std::move(other.m_UniformLocationCache);

        other.m_RendererID = 0;
    }
    return *this;
}

void Shader::bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

ShaderProgramSource Shader::parseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);
    if (!stream.is_open())
    {
        std::cout << "ERROR::SHADER: could not open file: " << filepath << std::endl;
        return { "", "" };
    }

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else if (type != ShaderType::NONE)   // guard: skip lines before the first #shader
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetShaderInfoLog(id, length, &length, message.data());

        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader in " << m_Filepath << std::endl;
        std::cout << message.data() << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetProgramInfoLog(program, length, &length, message.data());

        std::cout << "Failed to link program in " << m_Filepath << std::endl;
        std::cout << message.data() << std::endl;
    }

    glValidateProgram(program);

    // shaders are baked into the program now
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::getUniformLocation(const std::string &name)
{
    auto it = m_UniformLocationCache.find(name);
    if (it != m_UniformLocationCache.end())
        return it->second;

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' not found in "
                  << m_Filepath << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::setInt(const std::string &name, int value)
{
    glUseProgram(m_RendererID);
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value)
{
    glUseProgram(m_RendererID);
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value)
{
    glUseProgram(m_RendererID);
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUseProgram(m_RendererID);
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix)
{
    glUseProgram(m_RendererID);
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}