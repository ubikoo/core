#version 330 core

uniform float uWidth;
uniform float uHeight;
uniform sampler2D uTexSampler;

in vec4 vSphereNormal;
in vec4 vSphereColor;
in vec2 vSphereTexcoord;

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
    vec3 coord = getCoord();
    // vec4 tex_color = texture(uTexSampler, vSphereTexcoord);
    // vec4 pos_color = vSphereColor;
    // outFragColor = mix(tex_color, pos_color, 0.5);
    outFragColor = texture(uTexSampler, vSphereTexcoord);
}
