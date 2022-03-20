#ifndef CUBE_H
#define CUBE_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"

class Cube {
private:
    void init();
public:
    // constant quantities
    float mass;
    glm::mat3 inertia, inertiaInv;

    // state variables
    glm::vec3 position, velocity, omega;
    glm::quat quaternion;

    // computed quantities
    std::vector<glm::vec3> force, torque;
    glm::vec3 total_force, total_torque;

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    unsigned int vao, vbo, ibo;

    Cube();
    void update(const float delta_t);
    void addForce(glm::vec3 f, int pos);
    void draw(Shader& shader, glm::vec3 objectColor);
};

#endif // CUBE_H

