#version 330 core

in vec4 vert_col;
out vec4 frag_col;

void main()
{
    frag_col = vert_col;
}
