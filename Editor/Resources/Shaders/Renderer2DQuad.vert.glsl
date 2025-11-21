#version 460 core

layout(location = 0) in vec3 i_Positions;
layout(location = 1) in vec4 i_Color;
layout(location = 2) in vec2 i_TextureCoords;
layout(location = 3) in float i_TextureIndex;
layout(location = 4) in float i_TilingFactor;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec2 o_TextureCoords;
layout(location = 2) out float o_TextureIndex;
layout(location = 3) out float o_TilingFactor;

layout(location = 0) uniform mat4 u_ViewProjectionMatrix;
layout(location = 1) uniform mat4 u_Transform;

void main() {
    o_Color = i_Color;
    o_TextureCoords = i_TextureCoords;
    o_TextureIndex = i_TextureIndex;
    o_TilingFactor = i_TilingFactor;
    
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(i_Positions, 1.0);
}