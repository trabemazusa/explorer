#include <glm/gtx/matrix_operation.hpp>

#include "cloth.h"

Mass::Mass(glm::vec3 position, int index)
{
    m_mass = 1.0f;
    m_position = position;
    m_velocity = glm::vec3(0.0f);
    m_force = glm::vec3(0.0f);

    m_pinned = false;
    m_index = index;
}

Spring::Spring(Mass* m1, Mass* m2)
    : m_m1(m1),
      m_m2(m2)
{
    m_k = 1000.0f;
    m_rest_length = glm::distance(m1->m_position, m2->m_position);
} 

Cloth::Cloth(int rows, int columns)
{
    m_damping = 0.8f;
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

    // init jacobian
    jacobian_dfdx = std::vector<std::vector<glm::mat3>>(m_masses.size(), std::vector<glm::mat3>(m_masses.size(), glm::mat3(0.0f)));
    jacobian_dfdv = std::vector<std::vector<glm::mat3>>(m_masses.size(), std::vector<glm::mat3>(m_masses.size(), glm::mat3(0.0f)));
}

Cloth::~Cloth()
{
}

void Cloth::clear_force()
{
    for (auto &m : m_masses)
    {
        m->m_force = glm::vec3(0.0f);
    }
}

void Cloth::compute_force()
{
    clear_force();

    for (auto &m : m_masses)
    {
        m->m_force += m->m_mass * glm::vec3(0.0f, -9.8f, 0.0f);
    }

    for (auto &s : m_springs)
    {
        glm::vec3 x_ij = s->m_m1->m_position - s->m_m2->m_position;
        glm::vec3 v_ij = s->m_m1->m_velocity - s->m_m2->m_velocity;

        glm::vec3 x_ij_hat = glm::normalize(x_ij);
        float distance = glm::length(x_ij);

        // spring force
        glm::vec3 f_hooke = -s->m_k * (distance - s->m_rest_length) * x_ij_hat;
        s->m_m1->m_force += f_hooke;
        s->m_m2->m_force -= f_hooke;

        // damping force
        glm::vec3 f_damping = -m_damping * x_ij_hat * v_ij * x_ij_hat;
        s->m_m1->m_force += f_damping;
        s->m_m2->m_force -= f_damping;
    }
}

void Cloth::compute_jacobian()
{
    // remember to reset both Jacobian of dfdx and dfdv
    jacobian_dfdx = std::vector<std::vector<glm::mat3>>(m_masses.size(), std::vector<glm::mat3>(m_masses.size(), glm::mat3(0.0f)));
    jacobian_dfdv = std::vector<std::vector<glm::mat3>>(m_masses.size(), std::vector<glm::mat3>(m_masses.size(), glm::mat3(0.0f)));

    for (auto &s : m_springs)
    {
        glm::vec3 x_ij = s->m_m1->m_position - s->m_m2->m_position;
        glm::vec3 x_ij_hat = glm::normalize(x_ij);

        int i = s->m_m1->m_index;
        int j = s->m_m2->m_index;

        glm::mat3 x_ij_hat_matrix = glm::outerProduct(x_ij_hat, x_ij_hat);
        glm::mat3 dfdxi = -s->m_k * (glm::mat3(1.0f) - s->m_rest_length / glm::length(x_ij) * (glm::mat3(1.0f) - x_ij_hat_matrix));

        jacobian_dfdx[i][i] += dfdxi;
        jacobian_dfdx[i][j] -= dfdxi;
        jacobian_dfdx[j][i] -= dfdxi;
        jacobian_dfdx[j][j] += dfdxi;

        glm::mat3 dfdvi = -m_damping * x_ij_hat_matrix;

        jacobian_dfdv[i][i] += dfdvi;
        jacobian_dfdv[i][j] -= dfdvi;
        jacobian_dfdv[j][i] -= dfdvi;
        jacobian_dfdv[j][j] += dfdvi;
    }
}

// Jacobi Method
void Cloth::solve_linear_system(std::vector<std::vector<glm::mat3>>& A, std::vector<glm::vec3>& x, std::vector<glm::vec3>& b)
{
    // condition of convergence
    float residual_error = 0.0f;

    // A = D + R
    std::vector<glm::vec3> D(A.size(), glm::vec3(0.0f)); // diagonal of A
    std::vector<glm::vec3> Rx(m_masses.size(), glm::vec3(0.0f)); // R * x

    // the main iteration loop
    for (int k = 0; k < 32; ++k)
    {
        for (int i = 0; i < A.size(); ++i)
        {
            glm::vec3 Ax(0.0f);
            glm::vec3 single_residual_error(0.0f);

            for (int j = 0; j < A[i].size(); ++j)
            {
                Ax += A[i][j] * x[j];
                if (i == j)
                {
                    D[i] = glm::vec3(A[i][j][0][0], A[i][j][1][1], A[i][j][2][2]);
                    glm::mat3 diagonal_of_A = glm::diagonal3x3(D[i]);
                    Rx[i] += (A[i][j] - diagonal_of_A) * x[j]; // R = A - D
                }
                else
                {
                    Rx[i] += A[i][j] * x[j];
                }
            }

            // compute residual error for convergence detection
            single_residual_error = Ax - b[i];
            residual_error = single_residual_error.x * single_residual_error.x + 
                             single_residual_error.y * single_residual_error.y +
                             single_residual_error.z * single_residual_error.z;
        }


        // update x every iteration
        for (int i = 0; i < x.size(); ++i)
        {
            x[i] = glm::inverse(glm::diagonal3x3(D[i])) * (b[i] - Rx[i]); 
        }

        // break the iteration if converge
        if (std::sqrt(residual_error) < 0.1f) break;
    }
}

void Cloth::implicit_tick(float delta_t)
{
    compute_force();
    
    compute_jacobian();

    // A
    std::vector<std::vector<glm::mat3>> A(m_masses.size(), std::vector<glm::mat3>(m_masses.size(), glm::mat3(0.0f)));
    for (int i = 0; i < A.size(); ++i)
    {
        for (int j = 0; j < A[i].size(); ++j)
        {
            if (i == j)
            {
                A[i][j] = glm::mat3(1.0f) - delta_t * jacobian_dfdv[i][j] - delta_t * delta_t * jacobian_dfdx[i][j];
            }
            else
            {
                A[i][j] = glm::mat3(0.0f) - delta_t * jacobian_dfdv[i][j] - delta_t * delta_t * jacobian_dfdx[i][j];
            }
        }
    }

    // delta_v
    std::vector<glm::vec3> delta_v(m_masses.size(), glm::vec3(0.0f));

    // b
    std::vector<glm::vec3> b(m_masses.size(), glm::vec3(0.0f));
    for (int i = 0; i < m_masses.size(); ++i)
    {
        b[i] = delta_t * m_masses[i]->m_force;
        for (int j = 0; j < m_masses.size(); ++j)
        {
            b[i] += delta_t * delta_t * jacobian_dfdx[i][j] * m_masses[j]->m_velocity;
        }
    }

    solve_linear_system(A, delta_v, b);

    // update state
    for (int i = 0; i < m_masses.size(); ++i)
    {
        if (!m_masses[i]->m_pinned)
        {
            m_masses[i]->m_velocity += delta_v[i];
            m_masses[i]->m_position += delta_t * m_masses[i]->m_velocity;
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
