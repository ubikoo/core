#version 330 core

uniform mat4 u_mvp;

layout (location = 0) in vec3 model_position;
layout (location = 1) in vec3 model_normal;
layout (location = 2) in vec3 model_color;
layout (location = 3) in vec2 model_texcoord;

out vec4 vert_model_normal;
out vec4 vert_model_color;
out vec2 vert_model_texcoord;

void main()
{
    gl_Position = u_mvp * vec4(model_position, 1.0);
    vert_model_normal = vec4(model_normal, 1.0);
    vert_model_color = vec4(model_color, 1.0);
    vert_model_texcoord = model_texcoord;
}
