#version 330 core

uniform float u_width;
uniform float u_height;
uniform sampler2D u_texsampler;

in vec4 vert_model_normal;
in vec4 vert_model_color;
in vec2 vert_model_texcoord;

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
    vec3 normal = 0.5 + 0.5*vert_model_normal.xyz;
    frag_color = vec4(normal.x, normal.y, normal.z, 1);
}
