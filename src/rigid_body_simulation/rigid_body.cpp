
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "rigid_body.h"

RigidBody::RigidBody()
{
    pinned = false;
    gravity = true;
}

RigidBody::~RigidBody()
{
}

void RigidBody::init_physics()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    // calculate the center of mass
    for (int i = 0; i < vertices.size(); ++i)
    {
        position += vertices[i];
    }
    position /= vertices.size();
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    angular_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    quaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    mass = 1.0f;
    glm::mat3 inertia = glm::mat3(0.0f);
    for (int i = 0; i < vertices.size(); ++i)
    {
        glm::vec3 ref = vertices[i] - position;
        inertia += glm::mat3(ref.x * ref.x + ref.y * ref.y + ref.z * ref.z) - glm::outerProduct(ref, ref);

        // nbodies
        Body body = Body();
        body.force = glm::vec3(0.0f, 0.0f, 0.0f);
        body.torque = glm::vec3(0.0f, 0.0f, 0.0f);
        nbodies.push_back(body);
    }
    inertia = mass * inertia;
    inertia_inverse = glm::inverse(inertia);

    force = glm::vec3(0.0f, 0.0f, 0.0f);
    torque = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::add_force(glm::vec3 force, int body_index)
{
    nbodies[body_index].force += force;
}

void RigidBody::update(float delta_t)
{
    if (!pinned)
    {
        if (gravity)
        {
            force += mass * glm::vec3(0.0f, -9.8f, 0.0f);

            glm::mat3 rotation_matrix = glm::toMat3(quaternion);

            for (int i = 0; i < nbodies.size(); ++i)
            {
                glm::vec3 ref = vertices[i] - position;
                nbodies[i].torque = glm::cross(rotation_matrix * ref, nbodies[i].force);

                force += nbodies[i].force;
                torque += nbodies[i].torque;
            }

            glm::mat3 ref_inertia_inverse = rotation_matrix * inertia_inverse * glm::transpose(rotation_matrix);

            velocity += delta_t * force / mass;
            position += delta_t * velocity;

            angular_velocity += delta_t * ref_inertia_inverse * torque;
            quaternion += glm::cross(glm::quat(0.0f, delta_t / 2.0f * angular_velocity), quaternion);
            // Don't forget to normalize quaternion
            quaternion = glm::normalize(quaternion);

            // Impluse method for collision detection and response 
            for (auto& ri : vertices) 
            {
                glm::vec3 xi = position + rotation_matrix * ri; // xi is in world space
                // Distance Funtion
                // center of the ground is (0.0, -2.0, 0.0)
                // normal of the ground is (0.0,  1.0, 0.0)
                float distance = glm::dot((xi - glm::vec3(0.0f, -2.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
                if (distance < 0.0f) 
                {
                    glm::vec3 vi = velocity + glm::cross(angular_velocity, rotation_matrix * ri);
                    // the ground normal is (0.0, 1.0, 0.0)
                    glm::vec3 N = glm::vec3(0.0f, 1.0f, 0.0);

                    // avoid inter-penetration
                    position -= distance * N;

                    if (glm::dot(vi, N) < 0) 
                    {
                        // Compute auxiliary varialbe vi_new
                        glm::vec3 viN = glm::dot(vi, N) * N;
                        glm::vec3 viT = vi - viN;
                        float muT = 0.5f;
                        float muN = 0.5f;
                        float a = std::max(1 - muT * (1 + muN) * glm::length(viN) / glm::length(viT), 0.0f);
                        glm::vec3 vi_new = -muN * viN + a * viT;

                        // Compute the impluse j
                        glm::vec3 Rri = rotation_matrix * ri;
                        glm::mat3 matrix_product = glm::mat3(glm::vec3(0.0, Rri.z, -Rri.y), glm::vec3(-Rri.z, 0.0, Rri.x), glm::vec3(Rri.y, -Rri.x, 0.0));
                        glm::mat3 K = 1 / mass * glm::mat3(1.0f) - matrix_product * ref_inertia_inverse * matrix_product;
                        glm::vec3 j = glm::inverse(K) * (vi_new - vi);

                        // update velocity and omega
                        velocity += 1 / mass * j;
                        angular_velocity += ref_inertia_inverse * glm::cross(rotation_matrix * ri, j);
                    }
                }
            }

            // reset forces and torques
            force = glm::vec3(0.0f, 0.0f, 0.0f);
            torque = glm::vec3(0.0f, 0.0f, 0.0f);
            for (auto& body : nbodies)
            {
                body.force = glm::vec3(0.0f, 0.0f, 0.0f);
                body.torque = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            // update model matrix
            model_matrix = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, position);
            model_matrix = model_matrix * glm::toMat4(quaternion);
            model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }
}

void RigidBody::pin()
{
    pinned = true;
}

void RigidBody::un_pin()
{
    pinned = false;
}

