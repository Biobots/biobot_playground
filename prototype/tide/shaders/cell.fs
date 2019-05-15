#version 330 core

in float color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color, color, color, 1.0f);
}