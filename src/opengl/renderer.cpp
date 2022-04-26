#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"

#include <stdio.h>

Renderer::Renderer()
{    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to initialize OpenGL context.\n");
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer()
{
}

void Renderer::create_renderer(const char* vpath, const char* fpath)
{
    shader = new Shader(vpath, fpath);
}

void Renderer::create_default_renderer()
{
    shader = new Shader("../src/opengl/glsl/vertex.glsl", "../src/opengl/glsl/fragment.glsl");
}

void Renderer::set_camera_pos(glm::vec3 pos, int window_width, int window_height)
{
    glm::mat4 view = glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)window_width/(float)window_height, 0.1f, 100.0f);

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
}

void Renderer::clear_color(glm::vec3 color)
{
    glClearColor(color.x, color.y, color.z, 0.0f);
}

void Renderer::clear_color_buffer()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear_depth_buffer()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw_triangles(Shape* shape, bool draw_wireframe, bool update_vertex_data)
{
    glBindVertexArray(shape->vao);

    shader->use();

    shader->setMat4("model", shape->model_matrix);
    shader->setVec3("objectColor", shape->color);

    if (update_vertex_data)
    {
        if (!shape->vertices_ptr.empty())
        {
            for (int i = 0; i < shape->vertices_ptr.size(); ++i)
            {
                shape->vertices[i] = *shape->vertices_ptr[i];
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * shape->vertices.size(), shape->vertices.data());
    }

    if (draw_wireframe) 
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw wireframe
    }

    glDrawElements(GL_TRIANGLES, shape->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Renderer::draw_lines(Shape* shape)
{
    glBindVertexArray(shape->vao);

    shader->use();

    shader->setMat4("model", shape->model_matrix);
    shader->setVec3("objectColor", shape->color);

    glDrawElements(GL_LINES, shape->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Renderer::draw_line_strip(Shape* shape)
{
    glBindVertexArray(shape->vao);

    shader->use();

    shader->setMat4("model", shape->model_matrix);
    shader->setVec3("objectColor", shape->color);

    glDrawElements(GL_LINE_STRIP, shape->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Renderer::draw_points(Shape* shape, float point_size)
{
    glBindVertexArray(shape->vao);

    shader->use();

    shader->setMat4("model", shape->model_matrix);
    shader->setVec3("objectColor", shape->color);

    glPointSize(point_size);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * shape->vertices.size(), shape->vertices.data());
    glDrawElements(GL_POINTS, shape->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Renderer::destroy()
{
    shader->destroy();
    delete shader;
}
