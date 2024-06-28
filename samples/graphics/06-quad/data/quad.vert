#version 330 core

uniform mat4 uMvp;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inCol;
out vec4 vColor;

void main()
{
    gl_Position = uMvp * inPos;
    vColor = inCol;
}
