#pragma stage : vertex
#version 460 core

layout(location = 0) in vec3 i_Position;

void main()
{
    gl_Position = vec4(i_Position, 1.0);
}

#pragma stage : fragment
#version 460 core

layout(location = 0) out vec4 o_Color;

void main()
{
    o_Color = vec4(0.8, 0.3, 0.2, 1.0);
}