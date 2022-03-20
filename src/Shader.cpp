#include "Shader.h"

#include <fstream>
#include <sstream>
#include <string>

#include <Gl/glew.h>
#include <glm/glm.hpp>

Shader::Shader(const char* vpath, const char* fpath)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    loadShader(vertexShader, vpath);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShader(fragmentShader, fpath);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::setMat4(const char* attrib, glm::mat4& matrix)
{
    int attribute = glGetUniformLocation(shaderProgram, attrib);
    glUniformMatrix4fv(attribute, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const char* attrib, glm::vec3& vector)
{
    int attribute = glGetUniformLocation(shaderProgram, attrib);
    glUniform3fv(attribute, 1, &vector[0]);
}

void Shader::use()
{
    glUseProgram(shaderProgram);
}

void Shader::destroy()
{
    glDeleteProgram(shaderProgram);
}

void Shader::loadShader(unsigned int& shader_name, const char* path)
{
    std::ifstream file(path, std::ios::in);
    std::stringstream fileSource;

    if (file.is_open()) {
        fileSource << file.rdbuf();
    }
    else {
        printf("Shader source file failed to open!\n");
        exit(1);
    }

    file.close();

    std::string shaderCode = fileSource.str();
    const char* shaderSource = shaderCode.c_str();
    glShaderSource(shader_name, 1, &shaderSource, NULL);
    glCompileShader(shader_name);

    GLint shader_compiled;
    glGetShaderiv(shader_name, GL_COMPILE_STATUS, &shader_compiled);
    if (shader_compiled != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shader_name, 1024, &log_length, message);
        // Write the error to a log
        printf("%s", message);
        exit(1);
    }
}
