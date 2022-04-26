#include "window.h"

Window::Window(const char* title, int width, int height)
    : width(width), height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);

}

bool Window::window_should_close()
{
    return glfwWindowShouldClose(window);
}

bool Window::press_escape()
{
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

bool Window::press_space()
{
    return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
}

void Window::close_window()
{
    glfwSetWindowShouldClose(window, true);
}

void Window::swap_buffer()
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

int Window::get_window_width()
{
    return width;
}

int Window::get_window_height()
{
    return height;
}

void Window::destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

