#include "FileLoader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <Gl/glew.h>

void loadShader(unsigned int shader_name, const char* path)
{
    std::ifstream file(path, std::ios::in);
    std::stringstream fileSource;

    if (file.is_open()) {
        fileSource << file.rdbuf();
    }
    else {
        throw std::runtime_error("Shader source file failed to open!");
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
        throw std::runtime_error(message);
    }
}

