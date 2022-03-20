#ifndef PLANE_H
#define PLANE_H

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

class Plane {
public:
    std::vector<glm::vec3> vertices;
    unsigned int vao, vbo, ibo;

    Plane();
    void draw(Shader& shader, glm::vec3 objectColor);
};


#endif // PLANE_H
