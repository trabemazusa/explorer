#include <glm/gtx/matrix_operation.hpp>

#include "cloth.h"

Mass::Mass(glm::vec3 position, int index)
{
    m_mass = 1.0f;
    m_position = position;
    m_velocity = glm::vec3(0.0f);
    m_force = glm::vec3(0.0f);

    m_pinned = false;
}

Spring::Spring(Mass* m1, Mass* m2)
    : m_m1(m1),
      m_m2(m2)
{
    m_k = 2000.0f;
    m_rest_length = glm::distance(m_m1->m_position, m_m2->m_position);
} 

Cloth::Cloth(int rows, int columns)
{
    m_dampping = 1.0f;
    int index = 0;
    int num_particles = rows * columns;
    float edge_length = 0.1f;

    // partices construct
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            Mass* m = new Mass(glm::vec3(0.5f - j * edge_length, 0.5f, 0.0f + i * edge_length), index++);
            m_masses.push_back(m);
            vertices_ptr.push_back(&m->m_position);
        }
    }

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

            indices.push_back(i * rows + j);
            indices.push_back(i * rows + j + 1);
            indices.push_back((i + 1) * rows + j + 1);

            indices.push_back(i * rows + j);
            indices.push_back((i + 1) * rows + j);
            indices.push_back((i + 1) * rows + j + 1);
        }
    }

    // springs
    for (int i = 0; i < rows - 1; ++i)
    {
        for (int j = 0; j < columns - 1; ++j)
        {
            // row
            Spring* row = new Spring(m_masses[i * rows + j], m_masses[i * rows + j + 1]);
            m_springs.push_back(row);

            // column
            Spring* column = new Spring(m_masses[i * rows + j], m_masses[(i + 1) * rows + j]);
            m_springs.push_back(column);

            // diagonal
            Spring* diagonal_forward = new Spring(m_masses[i * rows + j + 1], m_masses[(i + 1) * rows + j]);
            m_springs.push_back(diagonal_forward);

            Spring* diagonal_backward = new Spring(m_masses[i * rows + j], m_masses[(i + 1) * rows + j + 1]);
            m_springs.push_back(diagonal_backward);
        }
    }
    // still have one row side and one column side
    for (int i = 0; i < rows - 1; ++i)
    {
        // row
        Spring* row = new Spring(m_masses[i * rows + (columns - 1)], m_masses[(i + 1) * rows + (columns - 1)]);
        m_springs.push_back(row);
    }
    for (int i = 0; i < columns - 1; ++i)
    {
        // column
        Spring* column = new Spring(m_masses[(rows - 1) * columns + i], m_masses[(rows - 1) * columns + (i + 1)]);
        m_springs.push_back(column);
    }

    // pinned the top left corner and top right corner
    m_masses[0]->m_pinned = true;
    m_masses[rows - 1]->m_pinned = true;
}

Cloth::~Cloth()
{
}

void Cloth::clear_force()
{
    for (auto &particle : m_masses)
    {
        particle->m_force = glm::vec3(0.0f);
    }
}

void Cloth::compute_force()
{
    clear_force();

    for (auto &particle : m_masses)
    {
        particle->m_force += particle->m_mass * glm::vec3(0.0f, -9.8f, 0.0f);
    }

    for (auto &s : m_springs)
    {
        float distance = glm::distance(s->m_m1->m_position, s->m_m2->m_position);
        glm::vec3 f_hooke = -s->m_k * (distance - s->m_rest_length) * glm::normalize(s->m_m1->m_position - s->m_m2->m_position);
        s->m_m1->m_force += f_hooke;
        s->m_m2->m_force += -f_hooke;
    }
}

void Cloth::explicit_tick(float delta_t)
{
    compute_force();

    for (auto &particle : m_masses)
    {
        if (!particle->m_pinned)
        {
            particle->m_velocity += delta_t * particle->m_force / particle->m_mass;
            particle->m_velocity *= std::exp(-delta_t * m_dampping); // dampping
            particle->m_position += delta_t * particle->m_velocity;
        }
    }
}

void Cloth::destroy()
{
    for (auto mass : m_masses)
    {
        delete mass;
    }

    for (auto spring : m_springs)
    {
        delete spring;
    }
}
