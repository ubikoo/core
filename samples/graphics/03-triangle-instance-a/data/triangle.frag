#version 330 core

uniform float u_width;
uniform float u_height;

in vec4 vert_col;
out vec4 frag_col;

vec3 get_coord()
{
    float du = 2.0 / u_width;
    float dv = 2.0 / u_height;
    float dw = 2.0;
    return vec3(gl_FragCoord.x*du-1.0,
                gl_FragCoord.y*dv-1.0,
                gl_FragCoord.z*dw-1.0);
}

void main()
{
    vec3 r = get_coord();
    frag_col = vert_col;
}
