#include <glad/glad.h>
#include<iostream>
#include <fstream>
#include <sstream>


#include "Shader.h"
#include "Renderer.h"



Shader::Shader(const std::string& filename)
    :m_FilePath(filename), m_RendererID(0)
{
    // Use the provided filename instead of hardcoding it
    ShaderProgramSource source = parseShader(filename);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}


/**
 * Parse the Shader source file to get the corespanding code
 *
 * This parsing is very basic and holde true for our simple exemples
 * where the two shaders are separated by the line #shader vertex or #sahder fragment to get the right one
 *
 * @param filePatch, the path to the file holding the two shders.
 * @return ShaderProgramSource holding both code source of both Shaders
 */
ShaderProgramSource Shader::parseShader(const std::string& filePath) {
    std::ifstream stream(filePath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/**
 * Abstraction of the Compilation of the Shader
 *
 * @param type, is it vertex or fragment.
 * @param source, The code of the shader as a string .
 *
 * @return id, an identifier of the shader compiled.
 */
unsigned int Shader::CompileShader(unsigned int type, std::string& source) {

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/**
 * Abstraction of Creating of a shader
 *
 * @param values Container whose values are summed.
 * @return sum of `values`, or 0.0 if `values` is empty.
 */
int Shader::CreateShader(std::string& vetrexShader, std::string fragementShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vetrexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragementShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

// Set Uniform
void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
unsigned int Shader::GetUniformLocation(const std::string& name)
{
    // Caching the location make the code faster in long term i think 
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "[Warnning] uniform: " << name << " doesn't exist !! be careful" << std::endl;

    m_UniformLocationCache[name] = location;

    return location;
}