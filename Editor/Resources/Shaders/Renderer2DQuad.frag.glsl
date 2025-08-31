#version 460 core

layout(location = 0) out vec4 o_OutColor;

layout(location = 0) in vec4 o_Color;
layout(location = 1) in vec2 o_TextureCoords;

layout(location = 2) uniform float u_TilingFactor;

uniform sampler2D u_Texture;

void main() {
    //o_OutColor = texture(u_Texture, o_TextureCoords * u_TilingFactor) * o_Color;
    o_OutColor = o_Color;
}