#include "particles.hpp"

#include <cmath>

Particles::Particles(glm::vec3 start, float rest_length, int nums)
{
    this->nums = nums;
    for (int i = 0; i < nums; ++i) {
        pos.push_back(glm::vec3(start.x - (float)i * rest_length, start.y, start.z));
        vel.push_back(glm::vec3(0.0f));
        pinned.push_back(false);
        force.push_back(glm::vec3(0.0f));
    }
    pinned.front() = true;
}

void Particles::loadParticles(std::vector<float> &vertices)
{
    for (auto &p : pos) {
        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(p.z);
    }
}

void Particles::update_vertices(std::vector<float> &vertices) {
    for (int i = 0; i < nums; ++i) {
        vertices[3 * i + 0] = pos[i].x;
        vertices[3 * i + 1] = pos[i].y;
        vertices[3 * i + 2] = pos[i].z;
    }
}

void Particles::update_pos(float dt) {
    for (int i = 0; i < nums; ++i) {
        if (!pinned[i]) {
            vel[i] += force[i] / mass * dt;
            vel[i] *= std::exp(-dt * damping);
            pos[i] += vel[i] * dt;
        }
        force[i] = glm::vec3(0.0f);
    }
}

void Particles::compute_force(float k, float rest_length, glm::vec3 gravity) {
    for (int i = 0; i < nums; ++i) {
        force[i] += gravity * mass;
    }

    for (int i = 0; i < nums - 1; ++i) {
        glm::vec3 dir = glm::normalize(pos[i+1] - pos[i]);
        float delta_x = glm::distance(pos[i+1], pos[i]) - rest_length;
        glm::vec3 f = k * delta_x * dir;
        force[i] += f;
        force[i+1] += -f;
    }
}

