#version 460 core

layout(location = 0) out vec4 o_OutColor;

layout(location = 0) in vec4 o_Color;
layout(location = 1) in vec2 o_TextureCoords;
layout(location = 2) in float o_TextureIndex;
layout(location = 3) in float o_TilingFactor;

uniform sampler2D u_Textures[32];

void main() {
    o_OutColor = texture(u_Textures[int(o_TextureIndex)], o_TextureCoords * o_TilingFactor) * o_Color;
}