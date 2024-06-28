#version 330 core

uniform mat4 uMvp;

layout (location = 0) in vec3 inModelPosition;
layout (location = 1) in vec3 inModelNormal;
layout (location = 2) in vec3 inModelColor;
layout (location = 3) in vec2 inModelTexcoord;

out vec4 vModelNormal;
out vec4 vModelColor;
out vec2 vModelTexcoord;

void main()
{
    gl_Position = uMvp * vec4(inModelPosition, 1.0);
    vModelNormal = vec4(inModelNormal, 1.0);
    vModelColor = vec4(inModelColor, 1.0);
    vModelTexcoord = inModelTexcoord;
}
