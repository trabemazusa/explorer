#ifndef ROPE_H
#define ROPE_H

#include "Particle.h"

#include <glm/glm.hpp>

class Rope : public Particle {
public:
    float k;
    float rest_length;
    float damping = 0.6f;
    std::vector<bool> pinned;

    Rope(glm::vec3 start, float rest_length, int nums, float k_spring, std::vector<float> &vertices);
    void compute_force(glm::vec3 gravity);
    void update(float dt, std::vector<float> &vertices);
};

#endif // ROPE_H
