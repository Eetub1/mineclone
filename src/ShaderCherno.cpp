#include "ShaderCherno.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string &filepath)
    : id(0), m_Filepath(filepath)
{
    ShaderProgramSource source = parseShader(filepath);
    id = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    if (id)
        glDeleteProgram(id);
}

Shader::Shader(Shader &&other) noexcept
    : id(other.id),
      m_Filepath(std::move(other.m_Filepath)),
      uniformLocationCache(std::move(other.uniformLocationCache))
{
    other.id = 0;
}

Shader& Shader::operator=(Shader &&other) noexcept
{
    if (this != &other)
    {
        if (id)
            glDeleteProgram(id);

        id = other.id;
        m_Filepath = std::move(other.m_Filepath);
        uniformLocationCache = std::move(other.uniformLocationCache);

        other.id = 0;
    }
    return *this;
}

void Shader::bind() const
{
    glUseProgram(id);
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

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::getUniformLocation(const std::string &name)
{
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end())
        return it->second;

    int location = glGetUniformLocation(id, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' not found in "
                  << m_Filepath << std::endl;
    }

    uniformLocationCache[name] = location;
    return location;
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::setInt(const std::string &name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value)
{
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}