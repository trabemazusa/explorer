#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <vector>

#include <glm/glm.hpp>

class Particles {
public:
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> vel;
    std::vector<glm::vec3> force;
    std::vector<bool> pinned;
    
    int nums;
    float mass = 0.1f;
    float damping = 0.6f;

    Particles(glm::vec3 start, float rest_length, int nums); // 2D Line Constructor
    void loadParticles(std::vector<float> &vertices);
    void update_vertices(std::vector<float> &vertices);
    void update_pos(float dt);
    void compute_force(float k, float rest_length, glm::vec3 gravity);
};


#endif // PARTICLES_HPP
