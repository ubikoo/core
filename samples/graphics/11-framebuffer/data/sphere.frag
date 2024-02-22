#version 330 core

uniform float u_width;
uniform float u_height;
uniform sampler2D u_texsampler;

in vec4 vert_sphere_normal;
in vec4 vert_sphere_color;
in vec2 vert_sphere_texcoord;

out vec4 frag_color;

void main()
{
    frag_color = texture(u_texsampler, vert_sphere_texcoord);
}
