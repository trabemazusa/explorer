#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "cube.h"

Cube::Cube()
{
    init();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Cube::init()
{
    vertices.push_back(glm::vec3(-0.1f, 0.1f, 0.0f));
    vertices.push_back(glm::vec3(-0.1f, -0.1f, 0.0f));
    vertices.push_back(glm::vec3(0.1f, 0.1f, 0.0f));
    vertices.push_back(glm::vec3(0.1f, -0.1f, 0.0f));

    indices = {
        0, 1, 2,
        1, 2, 3
    };

    mass = 1.0f;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    omega = glm::vec3(0.0f, 0.0f, 0.0f);
    quaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    inertia = glm::mat4(0.0f);
    inertiaInv = glm::mat4(0.0f);

    for (int i = 0; i < vertices.size(); ++i) {
        glm::vec3 ref = vertices[i] - position;
        inertia += glm::mat3(ref.x * ref.x + ref.y * ref.y + ref.z * ref.z) - glm::outerProduct(ref, ref);

        force.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        torque.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    inertia = mass * inertia;
    inertiaInv = glm::inverse(inertia);

    total_force = glm::vec3(0.0f, 0.0f, 0.0f);
    total_torque = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Cube::update(const float delta_t)
{
    total_force = mass * glm::vec3(0.0f, -9.8f, 0.0f);

    glm::mat3 rotationMatrix = glm::toMat3(quaternion);

    for (int i = 0; i < vertices.size(); ++i) {
        total_force += force[i];
        glm::vec3 ref = vertices[i] - position;
        torque[i] = glm::cross(rotationMatrix * ref, force[i]);
        total_torque += torque[i];
    }

    glm::mat3 Iinv = rotationMatrix * inertiaInv * glm::transpose(rotationMatrix);

    // simi-implicit Euler
    velocity += delta_t * total_force / mass;
    position += delta_t * velocity;

    omega += delta_t * Iinv * total_torque;
    quaternion += glm::cross(glm::quat(0.0f, delta_t / 2.0f * omega), quaternion);
    // Don't forget to normalize quaternion
    quaternion = glm::normalize(quaternion);

    // Impluse method for collision detection and response 
    for (auto ri : vertices) {
       glm::vec3 xi = position + rotationMatrix * ri; // xi is in world space
       // Distance Funtion
       float distance = glm::dot((xi - glm::vec3(0.0f, -2.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
       if (distance < 0.0f) {
           glm::vec3 vi = velocity + glm::cross(omega, rotationMatrix * ri);
           // the ground normal is (0.0, 1.0, 0.0)
           glm::vec3 N = glm::vec3(0.0f, 1.0f, 0.0);

           // avoid inter-penetration
           position -= distance * N;

           if (glm::dot(vi, N) < 0) {
               // Compute auxiliary varialbe vi_new
               glm::vec3 viN = glm::dot(vi, N) * N;
               glm::vec3 viT = vi - viN;
               float muT = 0.5f;
               float muN = 0.5f;
               float a = std::max(1 - muT * (1 + muN) * glm::length(viN) / glm::length(viT), 0.0f);
               glm::vec3 vi_new = -muN * viN + a * viT;

               // Compute the impluse j
               glm::vec3 Rri = rotationMatrix * ri;
               glm::mat3 matrixProduct = glm::mat3(glm::vec3(0.0, Rri.z, -Rri.y), glm::vec3(-Rri.z, 0.0, Rri.x), glm::vec3(Rri.y, -Rri.x, 0.0));
               glm::mat3 K = 1 / mass * glm::mat3(1.0f) - matrixProduct * Iinv * matrixProduct;
               glm::vec3 j = glm::inverse(K) * (vi_new - vi);

               // update velocity and omega
               velocity += 1 / mass * j;
               omega += Iinv * glm::cross(rotationMatrix * ri, j);
           }
       }
    }

    // Reset forces and torques
    total_force = glm::vec3(0.0f, 0.0f, 0.0f);
    total_torque = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < vertices.size(); ++i) {
        force[i] = glm::vec3(0.0f, 0.0f, 0.0f);
        torque[i] = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void Cube::addForce(glm::vec3 f, int pos)
{
    this->force[pos] = f;
}

void Cube::draw(Shader& shader, glm::vec3 objectColor)
{
    glBindVertexArray(vao);

    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::toMat4(quaternion);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    shader.setMat4("model", model);
    shader.setVec3("objectColor", objectColor);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw wireframe
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
