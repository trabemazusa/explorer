#include <fstream>
#include <sstream>
#include <string>
#include <Gl/glew.h>

#include "shader.hpp"


static std::string readFile(const char* path)
{
    std::ifstream file(path, std::ios::in);
    std::stringstream shaderSource;

    if (file.is_open()) {
        shaderSource << file.rdbuf();
    }
    else {
        throw std::runtime_error("Shader source file failed to open!");
    }

    file.close();
    return shaderSource.str();
}

static void getCompileInfo(unsigned int shader)
{
    GLint shader_compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);
    if (shader_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        // Write the error to a log
        throw std::runtime_error(message);
    }
}

unsigned int createShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    std::string vShaderCode = readFile(vertexShaderFile);
    const char* vShaderSource = vShaderCode.c_str();
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSource, NULL);
    glCompileShader(vShader);
    getCompileInfo(vShader);


    std::string fShaderCode = readFile(fragmentShaderFile);
    const char* fShaderSource = fShaderCode.c_str();
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSource, NULL);
    glCompileShader(fShader);
    getCompileInfo(fShader);


    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    GLint program_linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(shaderProgram, 1024, &log_length, message);
        // Write the error to a log
        throw std::runtime_error(message);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return shaderProgram;
}

