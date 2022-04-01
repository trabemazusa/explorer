# version 410 core

out vec4 outColor;

uniform vec3 objectColor;

void main()
{
    outColor = vec4(objectColor, 1.0f);
}
