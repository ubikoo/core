#version 330 core

uniform mat4 uMvp;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inCol;
layout (location = 2) in vec3 inOffset;

out vec4 vColor;

void main()
{
    vec4 offset = vec4(inOffset, 1.0);
    gl_Position = uMvp * (inPos + offset);
    vColor = inCol;
}
