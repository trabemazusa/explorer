// https://graphics.pixar.com/pbm2001/pdf/notesg.pdf

#include "../opengl/window.h"
#include "../opengl/renderer.h"
#include "rigid_body.h"

int main()
{
    Window w = Window("Rigid Body Simulation", 512, 512);
    
    Renderer r = Renderer();
    r.create_default_renderer();
    r.set_camera_pos(glm::vec3(0, 0, 5), w.get_window_width(), w.get_window_height());

    RigidBody* square = new RigidBody();
    square->bind_square();
    square->load_buffer_data();
    square->set_color(glm::vec3(1.0f, 0.5f, 0.3f));
    square->init_physics();
    square->add_force(glm::vec3(0.0f, 500.0f, 0.0f), 1);

    RigidBody* line = new RigidBody();
    line->bind_line();
    line->load_buffer_data();
    line->set_color(glm::vec3(1.0f, 0.5f, 0.3f));
    line->init_physics();
    line->pin();

    while(!w.window_should_close())
    {
        if (w.press_escape()) w.close_window();

        r.clear_color(glm::vec3(0.0f, 0.0f, 0.0f));
        r.clear_color_buffer();
        r.clear_depth_buffer();

        square->update(0.01f);
        r.draw_triangles(square, false, false);
        r.draw_lines(line);

        w.swap_buffer();
    }

    w.destroy();
    r.destroy();

    delete square;
    delete line;

    return 0;
}
