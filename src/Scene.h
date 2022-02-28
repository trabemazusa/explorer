#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>

class Scene {
public:
    glm::vec3 top_center;
    glm::vec3 bottom_center;

    glm::vec3 left_center;
    glm::vec3 right_center;
    
    glm::vec3 front_center;
    glm::vec3 back_center;

    glm::vec3 center;

    Scene(glm::vec3 center, float length, float width, float height, std::vector<float>& vertices);
};

#endif // SCENE_H
