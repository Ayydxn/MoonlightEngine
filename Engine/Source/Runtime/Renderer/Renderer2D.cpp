#include "MoonlightPCH.h"
#include "Renderer2D.h"
#include "Debug/Profiler.h"
#include "Renderer/Renderer.h"
#include "RHICore/RHI.h"
#include "RHICore/Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CRenderer2D::CRenderer2D(const CRenderer2DSpecification& Specification)
    : m_Specification(Specification), m_MaxVertices(Specification.MaxQuadsPerDrawCall * 4), m_MaxIndices(Specification.MaxQuadsPerDrawCall * 6)
{
    Initialize();
}

CRenderer2D::~CRenderer2D()
{
    Shutdown();
}

void CRenderer2D::Initialize()
{
    const CVertexBufferLayout QuadVertexBufferLayout =
    {
        { "Positions", EShaderDataType::Float3 },
        { "Color", EShaderDataType::Float4 },
        { "TextureCoords", EShaderDataType::Float2 },
        { "TextureIndex", EShaderDataType::Float },
        { "TilingFactor", EShaderDataType::Float }
    };

    constexpr uint32 WhiteTextureData = 0xFFFFFFFF;

    uint32* QuadIndices = new uint32[m_MaxIndices];
    int32 Offset = 0;
    
    for (uint32 i = 0; i < m_MaxIndices; i += 6)
    {
        QuadIndices[i + 0] = Offset + 0;
        QuadIndices[i + 1] = Offset + 1;
        QuadIndices[i + 2] = Offset + 2;

        QuadIndices[i + 3] = Offset + 2;
        QuadIndices[i + 4] = Offset + 3;
        QuadIndices[i + 5] = Offset + 0;

        Offset += 4;
    }
    
    m_QuadShader = CRenderer::GetShaderLibrary()->GetShader("Renderer2DQuad");
    m_QuadVertexBuffer = CRHI::GetFactory().CreateVertexBuffer(m_MaxVertices * sizeof(CQuadVertex));
    m_QuadIndexBuffer = CRHI::GetFactory().CreateIndexBuffer(QuadIndices, m_MaxIndices);
    m_QuadGraphicsPipeline = CRHI::GetFactory().CreateGraphicsPipeline({
        .Shader = m_QuadShader,
        .VertexBufferLayout = QuadVertexBufferLayout,
        .DebugName = "Renderer2D - Quads"
    });
    m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    
    m_GlobalUniformBuffer = CRHI::GetFactory().CreateUniformBuffer(sizeof(CGlobalUniforms), 0);
    
    m_WhiteTexture = CRHI::GetFactory().CreateTexture(CTextureSpecification {
        .Width = 1,
        .Height = 1
    });
    m_WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32));
    m_QuadVertexBufferBase = new CQuadVertex[m_MaxVertices];

    delete[] QuadIndices;

    m_TextureSlots[0] = m_WhiteTexture;
}

void CRenderer2D::Shutdown()
{
    delete[] m_QuadVertexBufferBase;
}

void CRenderer2D::BeginFrame(const glm::mat4& ViewProjectionMatrix)
{
    m_QuadVertexBufferPointer = m_QuadVertexBufferBase;
    m_QuadIndexCount = 0;
    
    m_CurrentViewProjectionMatrix = ViewProjectionMatrix;
    
    m_TextureSlotIndex = 1;
}

void CRenderer2D::EndFrame()
{
    const uint64 DataSize = reinterpret_cast<uint8*>(m_QuadVertexBufferPointer) - reinterpret_cast<uint8*>(m_QuadVertexBufferBase);
    m_QuadVertexBuffer->SetData(m_QuadVertexBufferBase, DataSize);
    
    Flush();
}

void CRenderer2D::Flush()
{
    // Guard against empty draw calls.
    // While OpenGL specification defines glDrawElements call with an index count of 0 as a no-op, behavior varies across GPU vendors and driver versions in practice.
    // NVIDIA drivers (particularly on laptop GPUs) may re-issue the previous frame's draw,
    // AMD drivers may produce undefined behavior, and some older OpenGL implementations may not handle it gracefully at all.
    // Skipping the draw call entirely is the safest and most performant approach regardless of platform.
    if (m_QuadIndexCount)
    {
        for (uint32 i = 0; i < m_TextureSlotIndex; i++)
            m_TextureSlots[i]->Bind(i);
    
        CGlobalUniforms GlobalUniforms;
        GlobalUniforms.ViewProjectionMatrix = m_CurrentViewProjectionMatrix;
    
        m_GlobalUniformBuffer->SetData(&GlobalUniforms, sizeof(CGlobalUniforms));
    
        FRenderPacket RenderPacket;
        RenderPacket.Shader = m_QuadShader;
        RenderPacket.VertexBuffer = m_QuadVertexBuffer;
        RenderPacket.IndexBuffer = m_QuadIndexBuffer;
        RenderPacket.GraphicsPipeline = m_QuadGraphicsPipeline;
        RenderPacket.UniformBuffer = m_GlobalUniformBuffer;

        CRenderer::DrawIndexed(RenderPacket, m_QuadIndexCount);
    
        m_Statistics.DrawCalls++;
    }
}

void CRenderer2D::StartNewBatch()
{
    EndFrame();
    
    m_QuadVertexBufferPointer = m_QuadVertexBufferBase;
    m_QuadIndexCount = 0;
    
    m_TextureSlotIndex = 1;
}

void CRenderer2D::EmitQuadVertices(const glm::mat4& Transform, const glm::vec4& Color, float TextureIndex, float TilingFactor)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    if (m_QuadIndexCount >= m_MaxIndices)
        StartNewBatch();
    
    constexpr glm::vec2 TextureCoords[4] = { { 0,0 }, { 1,0 }, { 1,1 }, { 0,1 } };
    
    for (int32 i = 0; i < 4; i++)
    {
        m_QuadVertexBufferPointer->Position = Transform * m_QuadVertexPositions[i];
        m_QuadVertexBufferPointer->Color = Color;
        m_QuadVertexBufferPointer->TextureCoords = TextureCoords[i];
        m_QuadVertexBufferPointer->TextureIndex = TextureIndex;
        m_QuadVertexBufferPointer->TilingFactor = TilingFactor;
        m_QuadVertexBufferPointer++;
    }
    
    m_QuadIndexCount += 6;
    m_Statistics.QuadCount++;
}

void CRenderer2D::DrawQuad(const glm::mat4& Transform, const glm::vec4& Color)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    EmitQuadVertices(Transform, Color, 0.0f, 1.0f);
}

void CRenderer2D::DrawQuad(const glm::mat4& Transform, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    constexpr glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    float TextureIndex = 0.0f;
    
    for (uint32 i = 1; i < m_TextureSlotIndex; i++)
    {
        if (m_TextureSlots[i] == Texture)
        {
            TextureIndex = static_cast<float>(i);
            break;
        }
    }
    
    if (TextureIndex == 0.0f)
    {
        TextureIndex = static_cast<float>(m_TextureSlotIndex);
        m_TextureSlots[static_cast<uint64>(TextureIndex)] = Texture;
        
        m_TextureSlotIndex++;
    }
    
    EmitQuadVertices(Transform, Color, TextureIndex, TilingFactor);
}

void CRenderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color)
{
    DrawQuad({ Position.x, Position.y, 0.0f }, Size, Color);
}

void CRenderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    const glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });
    
    DrawQuad(Transform, Color);
}

void CRenderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    DrawQuad({ Position.x, Position.y, 0.0f }, Size, Texture, TilingFactor);
}

void CRenderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    constexpr glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    const glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });
    
    DrawQuad(Transform, Texture, TilingFactor);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
{
    DrawRotatedQuad({ Position.x, Position.y, 0.0f }, Size, Rotation, Color);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    const glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0, 0, 1 }) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });
    
    DrawQuad(Transform, Color);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    DrawRotatedQuad({ Position.x, Position.y, 0.0f }, Size, Rotation, Texture, TilingFactor);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    MOONLIGHT_PROFILE_FUNCTION();

    const glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0, 0, 1 }) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });
    
    DrawQuad(Transform, Texture, TilingFactor);
}

void CRenderer2D::ResetStats()
{
    std::memset(&m_Statistics, 0, sizeof(CRenderer2DStatistics));
}
