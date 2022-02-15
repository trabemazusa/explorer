#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "particles.hpp"


static const int WIDTH = 512;
static const int HEIGHT = 512;

static const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();


    // shader
    unsigned int shaderProgram = createShaderProgram("../shader/vertex.vert", "../shader/fragment.frag");
    glUseProgram(shaderProgram);

    float rest_length = 0.1f;
    Particles rope = Particles(glm::vec3(0.0f, 0.5f, 0.0f), rest_length, 10);

    std::vector<float> vertices;
    rope.loadParticles(vertices);


    // vao
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    

    // buffer
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STREAM_DRAW);



    // Attribute
    int posAttrib = glGetAttribLocation(shaderProgram, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);


    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPointSize(5);

        rope.compute_force(1000.0f, rest_length, gravity);
        rope.update_pos(0.01f);

        rope.update_vertices(vertices);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glDrawArrays(GL_POINTS, 0, rope.nums);
        glDrawArrays(GL_LINE_STRIP, 0, rope.nums);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
