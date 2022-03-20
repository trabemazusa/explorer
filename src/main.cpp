#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Cube.h"
#include "Plane.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rigid Body Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader = Shader("../shader/vertex.vert", "../shader/fragment.frag");

    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    Cube cube = Cube();
    cube.addForce(glm::vec3(0.0f, 500.0f, 0.0f), 1);


    Plane plane = Plane();


    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        cube.update(0.01f);
        cube.draw(shader, glm::vec3(1.0f, 0.5f, 0.3f));

        plane.draw(shader, glm::vec3(0.2f, 0.8f, 0.4f));


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shader.destroy();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
