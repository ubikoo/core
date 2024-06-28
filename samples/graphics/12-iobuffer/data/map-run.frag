#version 330 core

uniform sampler2D uTexSampler;

in vec4 vQuadNormal;
in vec4 vQuadColor;
in vec2 vQuadTexcoord;

out vec4 outFragColor;

void main()
{
    vec2 src = vQuadTexcoord;

    // vec2 dst;
    // if (src.x < 0.5) {
    //     dst.x = 2.0*src.x;
    //     dst.y = 0.5*src.y;
    // } else {
    //     dst.x = 2.0 - 2.0*src.x;
    //     dst.y = 1.0 - 0.5*src.y;
    // }
    vec2 dst = vec2(src.x + src.y, src.x + 2.0*src.y);
    dst = mod(dst, 1.0);

    outFragColor = texture(uTexSampler, dst);
}
