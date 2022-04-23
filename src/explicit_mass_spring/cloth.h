#ifndef CLOTH_H
#define CLOTH_H

#include "../opengl/shape.h"

struct Mass
{
    float m_mass;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_force;

    bool m_pinned;

    Mass(glm::vec3 position, int index);
};

struct Spring
{
    float m_k;
    float m_rest_length;
    Mass *m_m1, *m_m2;

    Spring(Mass* m1, Mass* m2);
};

class Cloth : public Shape
{
public:
    float m_dampping;
    std::vector<Mass*> m_masses;
    std::vector<Spring*> m_springs;

    Cloth(int rows, int columns);
    ~Cloth();

    void clear_force();
    void compute_force();
    void explicit_tick(float delta_t);
    void destroy();
};

#endif // CLOTH_H

