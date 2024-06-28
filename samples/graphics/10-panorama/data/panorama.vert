#version 330 core

#define PI 3.141592653589793

uniform mat4 uMvp;

layout (location = 0) in vec3 inSpherePosition;
layout (location = 1) in vec3 inSphereNormal;
layout (location = 2) in vec3 inSphereColor;
layout (location = 3) in vec2 inSphereTexcoord;

out vec4 vSphereNormal;
out vec4 vSphereColor;
out vec2 vSphereTexcoord;

void main()
{

    gl_Position = uMvp * vec4(inSpherePosition, 1.0);
    vSphereNormal = vec4(inSphereNormal, 1.0);
    vSphereColor = vec4(inSphereColor, 1.0);
    vSphereTexcoord = inSphereTexcoord;
}
