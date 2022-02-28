# version 410 core

in vec3 pass_color;

out vec4 outColor;

void main()
{
    outColor = vec4(pass_color, 1.0f);
}
