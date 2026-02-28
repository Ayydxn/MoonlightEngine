/*----------------*/
/* -- Uniforms -- */
/*----------------*/

[[vk::binding(0, 0)]]
cbuffer Uniforms : register(b0)
{
    column_major float4x4 u_Transform;
    column_major float4x4 u_ViewProjectionMatrix;
}

Sampler2D u_Textures[32];

/*------------------*/
/* -- Structures -- */
/*------------------*/

struct VertexInput
{
    [vk::location(0)] float3 Position : POSITION;
    [vk::location(1)] float4 Color : COLOR;
    [vk::location(2)] float2 TextureCoords : TEXCOORD0;
    [vk::location(3)] float TextureIndex : TEXCOORD1;
    [vk::location(4)] float TilingFactor : TILING;
}

struct VertexOutput
{
    float4 Position : SV_POSITION;
    [vk::location(0)] float4 Color : COLOR;
    [vk::location(1)] float2 TextureCoords : TEXCOORD0;
    [vk::location(2)] float TextureIndex : TEXCOORD1;
    [vk::location(3)] float TilingFactor : TILING;
}

struct FragmentInput
{
    [vk::location(0)] float4 Color : COLOR;
    [vk::location(1)] float2 TextureCoords : TEXCOORD0;
    [vk::location(2)] float TextureIndex : TEXCOORD1;
    [vk::location(3)] float TilingFactor : TILING;
}

struct FragmentOutput
{
    [vk::location(0)] float4 Color : SV_Target;
}

[shader("vertex")]
VertexOutput VertexMain(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(u_ViewProjectionMatrix, mul(u_Transform, float4(input.Position, 1.0)));
    output.Color = input.Color;
    output.TextureCoords = input.TextureCoords;
    output.TextureIndex = input.TextureIndex;
    output.TilingFactor = input.TilingFactor;

    return output;
}

[shader("fragment")]
FragmentOutput FragmentMain(VertexOutput vertexOutput) : SV_Target
{
    FragmentOutput output;
    output.Color = u_Textures[int(vertexOutput.TextureIndex)].Sample(vertexOutput.TextureCoords * vertexOutput.TilingFactor) * vertexOutput.Color;
    
    return output;
}