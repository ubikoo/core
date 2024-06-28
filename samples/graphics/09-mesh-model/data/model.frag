#version 330 core

uniform float uWidth;
uniform float uHeight;
uniform sampler2D uTexSampler;

in vec4 vModelNormal;
in vec4 vModelColor;
in vec2 vModelTexcoord;

out vec4 outFragColor;

vec3 getCoord()
{
    float du = 2.0 / uWidth;
    float dv = 2.0 / uHeight;
    float dw = 2.0;
    return vec3(gl_FragCoord.x * du - 1.0,
                gl_FragCoord.y * dv - 1.0,
                gl_FragCoord.z * dw - 1.0);
}

void main()
{
    vec3 normal = 0.5 + 0.5*vModelNormal.xyz;
    outFragColor = vec4(normal.x, normal.y, normal.z, 1);
}
