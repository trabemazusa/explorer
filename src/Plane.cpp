#include <GL/glew.h>

#include "Plane.h"

Plane::Plane()
{
    vertices.push_back(glm::vec3(-3.0f, -2.0f, 0.0f));
    vertices.push_back(glm::vec3(3.0f, -2.0f, 0.0f));

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void Plane::draw(Shader& shader, glm::vec3 objectColor)
{
    glBindVertexArray(vao);

    shader.use();

    shader.setVec3("objectColor", objectColor);

    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    glDrawArrays(GL_LINES, 0, vertices.size());

    glBindVertexArray(0);
}
