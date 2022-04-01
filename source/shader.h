#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

class Shader {
private:
    unsigned int shaderProgram;
    void loadShader(unsigned int& shader_name, const char* path);

public:
    Shader(const char* vpath, const char* fpath);
    void setVec3(const char* attrib, glm::vec3& vector);
    void setMat4(const char* attrib, glm::mat4& matrix);
    void use();
    void destroy();
};

#endif // SHADER_H
