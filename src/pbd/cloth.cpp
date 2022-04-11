#include "cloth.h"

Edge::Edge(int p1, int p2)
    : p1(p1), p2(p2)
{
}

Cloth::Cloth()
{
    mass = 1.0f;
    rest_length = 0.1f;
    rows = 10;
    columns = 10;
    num_of_vertex = rows * columns;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            vertices.push_back(glm::vec3(0.5f - rest_length * j, 0.5f, 0.0f + rest_length * i));
            velocity.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }

    // indices buffer
    for (int i = 0; i < rows - 1; ++i)
    {
        for (int j = 0; j < columns - 1; ++j)
        {
            indices.push_back(i * rows + j);
            indices.push_back(i * rows + j + 1);
            indices.push_back((i + 1) * rows + j);

            indices.push_back(i * rows + j + 1);
            indices.push_back((i + 1) * rows + j);
            indices.push_back((i + 1) * rows + j + 1);
        }
    }

    // edges
    for (int i = 0; i < rows - 1; ++i)
    {
        for (int j = 0; j < columns - 1; ++j)
        {
            // row
            Edge row = Edge(i * rows + j, i * rows + (j + 1));
            edges.push_back(row);

            // column
            Edge column = Edge(i * rows + j, (i + 1) * rows + j);
            edges.push_back(column);

            // diagonal
            Edge diagonal = Edge(i * rows + (j + 1), (i + 1) * rows + j);
            edges.push_back(diagonal);
        }
    }
    // still have one row side and one column side
    for (int i = 0; i < rows - 1; ++i)
    {
        // row
        Edge row = Edge(i * rows + (columns - 1), (i + 1) * rows + (columns - 1));
        edges.push_back(row);
    }
    for (int i = 0; i < columns - 1; ++i)
    {
        // column
        Edge column = Edge((rows - 1) * columns + i, (rows - 1) * columns + (i + 1));
        edges.push_back(column);
    }
}

float Cloth::test_constraint(glm::vec3 p1, glm::vec3 p2)
{
    float constraint = glm::length(p1 - p2) - rest_length;
    return constraint;
}

void Cloth::tick(float delta_t)
{
    std::vector<glm::vec3> new_pos(vertices);
    for (int i = 0; i < vertices.size(); ++i)
    {
        // pinned vertex
        if (i == 0 || i == rows - 1) continue;

        velocity[i] += delta_t * glm::vec3(0.0f, -9.8f, 0.0f);
        new_pos[i] += delta_t * velocity[i];
    }

    // constraint solver
    float stiffness_stretch = 0.8f;
    int num_iteration = 10;
    float stiffness_stretch_prime = 1.0f - std::pow(1.0f - stiffness_stretch, 1.0f / (float)num_iteration);
    for (int iteration_count = 0; iteration_count < num_iteration; ++iteration_count)
    {
        for (int i = 0; i < edges.size(); ++i)
        {
            glm::vec3 p1 = new_pos[edges[i].p1];
            glm::vec3 p2 = new_pos[edges[i].p2];

            float constraint = test_constraint(p1, p2);
            glm::vec3 delta_p1 = stiffness_stretch_prime * -0.5f * constraint * glm::normalize(p1 - p2);
            glm::vec3 delta_p2 = -delta_p1;

            new_pos[edges[i].p1] += delta_p1;
            new_pos[edges[i].p2] += delta_p2;
        }
    }

    // update velocity and position
    for (int i = 0; i < vertices.size(); ++i)
    {
        // pinned vertex
        if (i == 0 || i == rows - 1) continue;

        velocity[i] = (new_pos[i] - vertices[i]) / delta_t;
        vertices[i] = new_pos[i];
    }
}

