#ifndef CLOTH_H
#define CLOTH_H

#include "../opengl/shape.h"

struct Edge
{
    int p1, p2;
    float m_rest_length;
    Edge(int p1, int p2);
};

class Cloth : public Shape
{
public:
    int rows;
    int columns;
    int num_of_vertex;
    float mass;
    float distance;
    std::vector<Edge> edges;
    std::vector<glm::vec3> velocity;

    Cloth();

    float test_constraint(glm::vec3 p1, glm::vec3 p2, float rest_length);
    void tick(float delta_t);
};

#endif // CLOTH_H
