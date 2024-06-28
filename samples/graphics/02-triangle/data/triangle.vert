#version 330 core

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inCol;
out vec4 vColor;

void main()
{
    gl_Position = inPos;
    vColor = inCol;
}
