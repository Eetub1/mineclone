#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_Filepath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    explicit Shader(const std::string &filepath);
    ~Shader();

    // non-copyable: the GL program handle is uniquely owned
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader &&other) noexcept;
    Shader& operator=(Shader &&other) noexcept;

    void bind() const;
    void unbind() const;

    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat4(const std::string &name, const glm::mat4 &matrix);

private:
    ShaderProgramSource parseShader(const std::string &filepath);
    unsigned int compileShader(unsigned int type, const std::string &source);
    unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader);
    int getUniformLocation(const std::string &name);
};