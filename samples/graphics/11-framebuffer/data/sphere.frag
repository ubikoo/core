#version 330 core

uniform float uWidth;
uniform float uHeight;
uniform sampler2D uTexSampler;

in vec4 vSphereNormal;
in vec4 vSphereColor;
in vec2 vSphereTexcoord;

out vec4 outFragColor;

void main()
{
    outFragColor = texture(uTexSampler, vSphereTexcoord);
}
