#include "Scene.h"

#include <vector>
#include <glm/glm.hpp>

Scene::Scene(glm::vec3 center, float length, float width, float height, std::vector<float>& vertices)
{
    this->center = center;

    this->left_center = center - glm::vec3(length / 2.0f, 0.0f, 0.0f);
    this->right_center = -left_center;

    this->top_center = center + glm::vec3(0.0f, height / 2.0f, 0.0f);
    this->bottom_center = -top_center;

    this->front_center = center + glm::vec3(0.0f, 0.0f, width / 2.0f);
    this->back_center = -front_center;

    float temp[] = {
        left_center.x,  top_center.y, front_center.z,
        left_center.x,  top_center.y, back_center.z,
        right_center.x, top_center.y, front_center.z,
        right_center.x, top_center.y, back_center.z,
        left_center.x,  top_center.y, front_center.z,
        right_center.x, top_center.y, front_center.z,
        left_center.x,  top_center.y, back_center.z,
        right_center.x, top_center.y, back_center.z,
        
        left_center.x,  bottom_center.y, front_center.z,
        left_center.x,  bottom_center.y, back_center.z,
        right_center.x, bottom_center.y, front_center.z,
        right_center.x, bottom_center.y, back_center.z,
        left_center.x,  bottom_center.y, front_center.z,
        right_center.x, bottom_center.y, front_center.z,
        left_center.x,  bottom_center.y, back_center.z,
        right_center.x, bottom_center.y, back_center.z,

        left_center.x,  top_center.y,    front_center.z,
        left_center.x,  bottom_center.y, front_center.z,
        left_center.x,  top_center.y,    back_center.z,
        left_center.x,  bottom_center.y, back_center.z,
        right_center.x, top_center.y,    front_center.z,
        right_center.x, bottom_center.y, front_center.z,
        right_center.x, top_center.y,    back_center.z,
        right_center.x, bottom_center.y, back_center.z,
    };

    for (int i = 0; i < 72; ++i) {
        vertices.push_back(temp[i]);
    }
}
