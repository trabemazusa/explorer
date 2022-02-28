#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "FileLoader.h"
#include "Rope.h"
#include "Scene.h"
#include "Draw.h"


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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animations", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();


    // Init
    std::vector<float> vertices_rope, vertices_scene;

    Rope rope = Rope(glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 15, 1000.0f, vertices_rope);
    Scene scene = Scene(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 2.0f, 2.0f, vertices_scene);


    // Set mvp
    glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1.0f);


    // vao_scene
    unsigned int vao_scene;
    glGenVertexArrays(1, &vao_scene);
    glBindVertexArray(vao_scene);

    // scene buffer
    unsigned int vbo_scene;
    glGenBuffers(1, &vbo_scene);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_scene);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_scene.size(), vertices_scene.data(), GL_STATIC_DRAW);


    // vao_rope
    unsigned int vao_rope;
    glGenVertexArrays(1, &vao_rope);
    glBindVertexArray(vao_rope);

    // rope buffer
    unsigned int vbo_rope;
    glGenBuffers(1, &vbo_rope);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_rope);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_rope.size(), vertices_rope.data(), GL_STREAM_DRAW);


    // create shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    loadShader(vertexShader, "../shader/vertex.vert");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShader(fragmentShader, "../shader/fragment.frag");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPointSize(5);

        set_camera(shaderProgram, model, view, projection);
        draw_scene(shaderProgram, vao_scene, vbo_scene, glm::vec3(0.0f, 0.0f, 1.0f), vertices_scene.size());


        rope.compute_force(gravity);
        rope.update(0.01f, vertices_rope);
        draw_rope(shaderProgram, vao_rope, vbo_rope, glm::vec3(1.0f, 1.0f, 0.0f), vertices_rope, rope.nums);


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vbo_rope);
    glDeleteBuffers(1, &vbo_scene);

    glDeleteVertexArrays(1, &vao_rope);
    glDeleteVertexArrays(1, &vao_scene);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
