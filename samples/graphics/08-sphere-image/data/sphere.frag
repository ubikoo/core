#version 330 core

uniform float u_width;
uniform float u_height;
uniform sampler2D u_texsampler;

in vec4 vert_sphere_normal;
in vec4 vert_sphere_color;
in vec2 vert_sphere_texcoord;

out vec4 frag_color;

vec3 get_coord()
{
    float du = 2.0 / u_width;
    float dv = 2.0 / u_height;
    float dw = 2.0;
    return vec3(gl_FragCoord.x * du - 1.0,
                gl_FragCoord.y * dv - 1.0,
                gl_FragCoord.z * dw - 1.0);
}

void main()
{
    vec3 coord = get_coord();
    vec4 tex_color = texture(u_texsampler, vert_sphere_texcoord);
    vec4 pos_color = vert_sphere_color;
    frag_color = mix(tex_color, pos_color, 0.5);
}
