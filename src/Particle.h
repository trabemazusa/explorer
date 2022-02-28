#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include <glm/glm.hpp>

class Particle {
public:
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> vel;
    std::vector<glm::vec3> force;
    
    int nums;
    float mass = 0.1f;
};


#endif // PARTICLE_H
