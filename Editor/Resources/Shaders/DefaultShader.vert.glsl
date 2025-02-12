#version 460 core

layout (location = 0) in vec3 i_Positions;
layout (location = 1) in vec2 i_TextureCoords;

layout (location = 0) out vec2 o_TextureCoords;

layout(location = 0) uniform mat4 u_ViewProjectionMatrix;

void main() {
    o_TextureCoords = i_TextureCoords;
    
    gl_Position = u_ViewProjectionMatrix * vec4(i_Positions, 1.0);
}