#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform float pointSize;
uniform float inColor;

out float color;

void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, 0.0, 1.0);

    gl_Position = model * pos;
    gl_PointSize = pointSize;
    color = inColor;
}