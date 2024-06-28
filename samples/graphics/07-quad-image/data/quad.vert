#version 330 core

uniform mat4 uMvp;

layout (location = 0) in vec3 inQuadPosition;
layout (location = 1) in vec3 inQuadNormal;
layout (location = 2) in vec3 inQuadColor;
layout (location = 3) in vec2 inQuadTexcoord;

out vec4 vQuadNormal;
out vec4 vQuadColor;
out vec2 vQuadTexcoord;

void main()
{
    gl_Position = uMvp * vec4(inQuadPosition, 1.0);
    vQuadNormal = vec4(inQuadNormal, 1.0);
    vQuadColor = vec4(inQuadColor, 1.0);
    vQuadTexcoord = inQuadTexcoord;
}
