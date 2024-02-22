#version 330 core

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec4 a_col;
out vec4 vert_col;

void main()
{
    gl_Position = a_pos;
    vert_col = a_col;
}
