#version 460 core

layout(location = 0) out vec4 o_OutColor;

layout (location = 0) in vec2 o_TextureCoords;

uniform sampler2D u_Texture;

void main() {
    o_OutColor = texture(u_Texture, o_TextureCoords);
}