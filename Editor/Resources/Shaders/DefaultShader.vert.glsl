﻿#version 460 core

layout (location = 0) in vec3 i_Positions;

layout(location = 0) uniform mat4 u_ViewProjectionMatrix;
layout(location = 1) uniform mat4 u_Transform;

void main() {
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(i_Positions, 1.0);
}