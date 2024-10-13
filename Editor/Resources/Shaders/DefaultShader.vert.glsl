#version 460 core

layout (location = 0) in vec3 i_Positions;

void main() {
    gl_Position = vec4(i_Positions, 1.0);
}