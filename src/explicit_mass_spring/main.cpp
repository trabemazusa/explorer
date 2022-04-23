
#include "../opengl/window.h"
#include "../opengl/renderer.h"

#include "cloth.h"


int main()
{
    Window w = Window("Explicit Mass Spring", 512, 512);

    Renderer r = Renderer();
    r.create_default_renderer();
    r.set_camera_pos(glm::vec3(2, 1, 2), w.get_window_width(), w.get_window_height());

    Cloth* cloth = new Cloth(10, 10);
    cloth->load_buffer_data();
    cloth->set_color(glm::vec3(1.0f, 0.5f, 0.3f));

    while (!w.window_should_close())
    {
        if (w.press_escape()) w.close_window();

        r.clear_color(glm::vec3(0.0f, 0.0f, 0.0f));
        r.clear_color_buffer();
        r.clear_depth_buffer();

        r.draw_triangles(cloth, true, true);
        cloth->explicit_tick(0.01f);

        w.swap_buffer();
    }

    w.destroy();
    r.destroy();

    cloth->destroy();
    delete cloth;

    return 0;
}
