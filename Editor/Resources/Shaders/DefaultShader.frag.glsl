﻿#version 460 core

layout(location = 0) out vec4 o_OutColor;

layout (location = 0) in vec2 o_TextureCoords;

layout (location = 2) uniform vec4 u_Color;

void main() {
    o_OutColor = u_Color;
}