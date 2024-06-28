#version 330 core

uniform float uWidth;
uniform float uHeight;
uniform sampler2D uTexSampler;

in vec4 vQuadNormal;
in vec4 vQuadColor;
in vec2 vQuadTexcoord;

out vec4 outFragColor;

// Compute fragment position in normalized device coordinates.
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
    vec3 coord = getCoord();
    vec4 tex_color = texture(uTexSampler, vQuadTexcoord);
    vec4 pos_color = vQuadColor;
    outFragColor = mix(tex_color, pos_color, 0.5);
}
