#ifndef DRAW_H
#define DRAW_H

#include <vector>

#include <glm/glm.hpp>

void set_camera(unsigned int& shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection);
void draw_scene(unsigned int& shaderProgram, unsigned int& vao, unsigned int& vbo, glm::vec3 color, int nums_vertices);
void draw_rope(unsigned int& shaderProgram, unsigned int& vao, unsigned int& vbo, glm::vec3 color, std::vector<float>& vertices_rope, int nums_particles);

#endif // DRAW_H

