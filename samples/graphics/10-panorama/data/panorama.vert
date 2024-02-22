#version 330 core

#define PI 3.141592653589793

uniform mat4 u_mvp;

layout (location = 0) in vec3 sphere_position;
layout (location = 1) in vec3 sphere_normal;
layout (location = 2) in vec3 sphere_color;
layout (location = 3) in vec2 sphere_texcoord;

out vec4 vert_sphere_normal;
out vec4 vert_sphere_color;
out vec2 vert_sphere_texcoord;

void main()
{

    gl_Position = u_mvp * vec4(sphere_position, 1.0);
    vert_sphere_normal = vec4(sphere_normal, 1.0);
    vert_sphere_color = vec4(sphere_color, 1.0);
    vert_sphere_texcoord = sphere_texcoord;
}