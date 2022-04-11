#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include "shader.h"
#include "renderer.h"

class Window
{
private:
    int width, height;
    GLFWwindow* window;
public:
    Window(const char* title, int width, int height);

    bool window_should_close();
    bool press_escape();
    bool press_space();
    void close_window();
    void swap_buffer();
    int get_window_width();
    int get_window_height();
    void destroy_window();

};


#endif // WINDOW_H

