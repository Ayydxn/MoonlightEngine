#include "MoonlightPCH.h"
#include "Renderer2D.h"
#include "Debug/Profiler.h"
#include "Renderer/Renderer.h"
#include "RHICore/Shader.h"
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
    const CVertexBufferLayout VertexBufferLayout =
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
    
    int32 TextureSamplers[MaxTextureSlots];
    for (int32 i = 0; i < MaxTextureSlots; i++)
        TextureSamplers[i] = i;
    
    m_QuadShader = CRenderer::GetShaderLibrary()->GetShader("Renderer2DQuad");
    m_QuadShader->SetIntArray("u_Textures", TextureSamplers, MaxTextureSlots);
    m_QuadVertexBuffer = IVertexBuffer::Create(m_MaxVertices * sizeof(CQuadVertex));
    m_QuadIndexBuffer = IIndexBuffer::Create(QuadIndices, m_MaxIndices);
    m_QuadGraphicsPipeline = IGraphicsPipeline::Create({
        .Shader = m_QuadShader,
        .VertexBufferLayout = VertexBufferLayout,
        .DebugName = "Renderer2D - Quads"
    });
    m_WhiteTexture = ITexture::Create(1, 1);
    m_WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32));
    m_QuadVertexBufferBase = new CQuadVertex[m_MaxVertices];

    delete[] QuadIndices;

    m_TextureSlots[0] = m_WhiteTexture;
}

void CRenderer2D::Shutdown()
{
    delete m_QuadVertexBufferBase;
}

void CRenderer2D::BeginFrame()
{
    m_QuadVertexBufferPointer = m_QuadVertexBufferBase;
    m_QuadIndexCount = 0;
    
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
    for (uint32 i = 0; i < m_TextureSlotIndex; i++)
        m_TextureSlots[i]->Bind(i);
    
    CRenderPacket RenderPacket;
    RenderPacket.Shader = m_QuadShader;
    RenderPacket.Shader->SetFloat("u_TilingFactor", 1.0f);
    RenderPacket.VertexBuffer = m_QuadVertexBuffer;
    RenderPacket.IndexBuffer = m_QuadIndexBuffer;
    RenderPacket.GraphicsPipeline = m_QuadGraphicsPipeline;
    RenderPacket.Texture = m_WhiteTexture;

    CRenderer::DrawIndexed(RenderPacket, glm::mat4(1.0f), m_QuadIndexCount);
}

void CRenderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color)
{
    DrawQuad({ Position.x, Position.y, 0.0f }, Size, Color);
}

void CRenderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    constexpr float WhiteTextureIndex = 0.0f;
    
    m_QuadVertexBufferPointer->Position = Position;
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 0.0f, 0.0f };
    m_QuadVertexBufferPointer->TextureIndex = WhiteTextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = 1.0f;
    m_QuadVertexBufferPointer++;

    m_QuadVertexBufferPointer->Position = { Position.x + Size.x, Position.y, 0.0f };
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 1.0f, 0.0f };
    m_QuadVertexBufferPointer->TextureIndex = WhiteTextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = 1.0f;
    m_QuadVertexBufferPointer++;

    m_QuadVertexBufferPointer->Position = { Position.x + Size.x, Position.y + Size.y, 0.0f };
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 1.0f, 1.0f };
    m_QuadVertexBufferPointer->TextureIndex = WhiteTextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = 1.0f;
    m_QuadVertexBufferPointer++;

    m_QuadVertexBufferPointer->Position = { Position.x, Position.y + Size.y, 0.0f };
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 0.0f, 1.0f };
    m_QuadVertexBufferPointer->TextureIndex = WhiteTextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = 1.0f;
    m_QuadVertexBufferPointer++;
    
    m_QuadIndexCount += 6;
}

void CRenderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    DrawQuad({ Position.x, Position.y, 0.0f }, Size, Texture, TilingFactor);
}

void CRenderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
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
    
    m_QuadVertexBufferPointer->Position = Position;
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 0.0f, 0.0f };
    m_QuadVertexBufferPointer->TextureIndex = TextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = TilingFactor;
    m_QuadVertexBufferPointer++;

    m_QuadVertexBufferPointer->Position = { Position.x + Size.x, Position.y, 0.0f };
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 1.0f, 0.0f };
    m_QuadVertexBufferPointer->TextureIndex = TextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = TilingFactor;
    m_QuadVertexBufferPointer++;

    m_QuadVertexBufferPointer->Position = { Position.x + Size.x, Position.y + Size.y, 0.0f };
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 1.0f, 1.0f };
    m_QuadVertexBufferPointer->TextureIndex = TextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = TilingFactor;
    m_QuadVertexBufferPointer++;

    m_QuadVertexBufferPointer->Position = { Position.x, Position.y + Size.y, 0.0f };
    m_QuadVertexBufferPointer->Color = Color;
    m_QuadVertexBufferPointer->TextureCoords = { 0.0f, 1.0f };
    m_QuadVertexBufferPointer->TextureIndex = TextureIndex;
    m_QuadVertexBufferPointer->TilingFactor = TilingFactor;
    m_QuadVertexBufferPointer++;
    
    m_QuadIndexCount += 6;
}

void CRenderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
{
    DrawRotatedQuad({ Position.x, Position.y, 0.0f }, Size, Rotation, Color);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    CRenderPacket RenderPacket;
    RenderPacket.Shader = m_QuadShader;
    RenderPacket.Shader->SetVector4f("u_Color", Color);
    RenderPacket.Shader->SetFloat("u_TilingFactor", 1.0f);
    RenderPacket.VertexBuffer = m_QuadVertexBuffer;
    RenderPacket.IndexBuffer = m_QuadIndexBuffer;
    RenderPacket.GraphicsPipeline = m_QuadGraphicsPipeline;
    RenderPacket.Texture = m_WhiteTexture;

    const auto Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });

    CRenderer::DrawIndexed(RenderPacket, Transform);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    DrawRotatedQuad({ Position.x, Position.y, 0.0f }, Size, Rotation, Texture, TilingFactor);
}

void CRenderer2D::DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor)
{
    MOONLIGHT_PROFILE_FUNCTION();
    
    CRenderPacket RenderPacket;
    RenderPacket.Shader = m_QuadShader;
    RenderPacket.Shader->SetVector4f("u_Color", glm::vec4(1.0f));
    RenderPacket.Shader->SetFloat("u_TilingFactor", TilingFactor);
    RenderPacket.VertexBuffer = m_QuadVertexBuffer;
    RenderPacket.IndexBuffer = m_QuadIndexBuffer;
    RenderPacket.GraphicsPipeline = m_QuadGraphicsPipeline;
    RenderPacket.Texture = Texture;

    const auto Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });
    
    CRenderer::DrawIndexed(RenderPacket, Transform);
}
