#include <glad/glad.h>

#include "shape.h"

Shape::Shape()
{    
    vertices = std::vector<glm::vec3>();
    vertices_ptr = std::vector<glm::vec3*>();
    normals = std::vector<glm::vec3>();
    indices = std::vector<unsigned int>();
    model_matrix = glm::mat4(1.0f);
    color = glm::vec3(0.0f, 0.0f, 0.0f);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
}

Shape::~Shape()
{
}

void Shape::bind_square()
{
    vertices = 
    {
        glm::vec3(-0.1f,  0.1f, 0.0f),
        glm::vec3(-0.1f, -0.1f, 0.0f),
        glm::vec3( 0.1f,  0.1f, 0.0f),
        glm::vec3( 0.1f, -0.1f, 0.0f)
    };

    normals = 
    {
    };

    indices = 
    {
        0, 1, 2,
        1, 2, 3
    };
}

void Shape::bind_line()
{
    vertices = 
    {
        glm::vec3(-5.0f, -2.0f, 0.0f),
        glm::vec3( 5.0f, -2.0f, 0.0f)
    };

    normals = 
    {
        glm::vec3(0.0f, 1.0f, 0.0f),
    };

    indices = 
    {
        0, 1
    };
}

void Shape::set_color(glm::vec3 color)
{
    this->color = color;
}

void Shape::load_buffer_data()
{    
    for (auto &p : vertices_ptr)
    {
        vertices.push_back(*p);
    }

    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

