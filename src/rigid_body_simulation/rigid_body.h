#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <vector>

#include <glm/gtx/quaternion.hpp>

#include "../opengl/shape.h"

struct Body
{
    glm::vec3 force;
    glm::vec3 torque;
};

class RigidBody : public Shape
{
public:
    RigidBody();
    ~RigidBody();

    float mass;
    glm::mat3 inertia_inverse;

    glm::vec3 position; 
    glm::vec3 velocity; 
    glm::vec3 angular_velocity;
    glm::quat quaternion;

    glm::vec3 force;
    glm::vec3 torque;

    std::vector<Body> nbodies;

    bool pinned;
    bool gravity;

    void init_physics();
    void add_force(glm::vec3 force, int body_index);
    void update(float delta_t);
    void pin();
    void un_pin();
};

#endif // RIGID_BODY_H
