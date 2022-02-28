#include "Draw.h"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

void set_camera(unsigned int& shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    int modelAttrib = glGetUniformLocation(shaderProgram, "u_model");
    glUniformMatrix4fv(modelAttrib, 1, GL_FALSE, &model[0][0]);

    int viewAttrib = glGetUniformLocation(shaderProgram, "u_view");
    glUniformMatrix4fv(viewAttrib, 1, GL_FALSE, &view[0][0]);

    int projectionAttrib = glGetUniformLocation(shaderProgram, "u_projection");
    glUniformMatrix4fv(projectionAttrib, 1, GL_FALSE, &projection[0][0]);
}

void draw_scene(unsigned int& shaderProgram, unsigned int& vao, unsigned int& vbo, glm::vec3 color, int nums_vertices)
{
    glUseProgram(shaderProgram);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int positionAttrib = glGetAttribLocation(shaderProgram, "in_position");
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    int colorAttrib = glGetUniformLocation(shaderProgram, "u_color");
    glUniform3f(colorAttrib, color.x, color.y, color.z);

    glDrawArrays(GL_LINES, 0, nums_vertices);
}

void draw_rope(unsigned int& shaderProgram, unsigned int& vao, unsigned int& vbo, glm::vec3 color, std::vector<float>& vertices_rope, int nums_particles)
{
    glUseProgram(shaderProgram);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_rope.size() * sizeof(float), vertices_rope.data());

    int positionAttrib = glGetAttribLocation(shaderProgram, "in_position");
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    int colorAttrib = glGetUniformLocation(shaderProgram, "u_color");
    glUniform3f(colorAttrib, color.x, color.y, color.z);
    
    glDrawArrays(GL_POINTS, 0, nums_particles);
    glDrawArrays(GL_LINE_STRIP, 0, nums_particles);
}

