#version 330 core

uniform vec3 uOffset[1000];
uniform mat4 uMvp;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inCol;

out vec4 vColor;

void main()
{
    vec4 offset = vec4(uOffset[gl_InstanceID], 1.0);
    gl_Position = uMvp * (inPos + offset);
    vColor = inCol;
}
