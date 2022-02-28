# version 410 core

in vec3 in_position;

out vec3 pass_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec3 u_color;


void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0f);
    pass_color = u_color;
}
