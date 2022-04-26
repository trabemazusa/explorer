#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include <glm/glm.hpp>

class Shape
{
public:
    Shape();
    virtual ~Shape();

    std::vector<glm::vec3*> vertices_ptr;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    glm::mat4 model_matrix;
    glm::vec3 color;
    unsigned int vao, vbo, ibo;

    void bind_square();
    void bind_line();
    void set_color(glm::vec3 color);
    void load_buffer_data();
};

#endif // SHAPE_H
