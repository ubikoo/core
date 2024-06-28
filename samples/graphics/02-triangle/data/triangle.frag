#version 330 core

uniform float uWidth;
uniform float uHeight;

in vec4 vColor;
out vec4 outFragColor;

vec3 getCoord()
{
    float du = 2.0 / uWidth;
    float dv = 2.0 / uHeight;
    float dw = 2.0;
    return vec3(gl_FragCoord.x*du-1.0,
                gl_FragCoord.y*dv-1.0,
                gl_FragCoord.z*dw-1.0);
}

void main()
{
    vec3 r = getCoord();
    outFragColor = vColor;
}
