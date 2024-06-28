#version 330 core

uniform sampler2D uTexSampler;

in vec4 vQuadNormal;
in vec4 vQuadColor;
in vec2 vQuadTexcoord;

out vec4 outFragColor;

void main()
{
    outFragColor = texture(uTexSampler, vQuadTexcoord);
}
