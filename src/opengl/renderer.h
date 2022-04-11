#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

#include "shader.h"
#include "shape.h"

class Renderer
{
private:
    Shader* shader;
public:
    Renderer();
    ~Renderer();

    // path shoule be absolute or relative to the .exe file
    void create_renderer(const char* vpath, const char* fpath);
    void create_default_renderer();
    void set_camera_pos(glm::vec3 pos, int window_width, int window_height);

    void clear_color(glm::vec3 color);
    void clear_depth_buffer();
    void clear_color_buffer();

    void draw_triangles(Shape* shape, bool draw_wireframe, bool update_vertex_data);
    void draw_lines(Shape* shape);
    void draw_line_strip(Shape* shape);
    void draw_points(Shape* shape, float point_size);

    void destroy_renderer();
};

#endif // RENDERER_H
