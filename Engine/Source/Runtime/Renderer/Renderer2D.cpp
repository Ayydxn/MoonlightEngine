#include "MoonlightPCH.h"
#include "Renderer2D.h"
#include "Renderer/Renderer.h"
#include "RHICore/Shader.h"
#include "RHICore/Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CRenderer2DData
{
    std::shared_ptr<IShader> QuadShader;
    std::shared_ptr<IVertexBuffer> QuadVertexBuffer;
    std::shared_ptr<IIndexBuffer> QuadIndexBuffer;
    std::shared_ptr<IGraphicsPipeline> QuadGraphicsPipeline;
};

static CRenderer2DData* s_Renderer2DData;

void CRenderer2D::Initialize()
{
    constexpr float QuadVertices[20] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    constexpr uint32 QuadIndices[] = { 0, 1, 2, 2, 3, 0 };

    const CVertexBufferLayout VertexBufferLayout =
    {
        { "Positions", EShaderDataType::Float3 },
        { "TextureCoords", EShaderDataType::Float2 }
    };

    s_Renderer2DData = new CRenderer2DData();
    s_Renderer2DData->QuadShader = CRenderer::GetShaderLibrary()->GetShader("Renderer2DQuad");
    s_Renderer2DData->QuadVertexBuffer = IVertexBuffer::Create(QuadVertices, sizeof(QuadVertices));
    s_Renderer2DData->QuadIndexBuffer = IIndexBuffer::Create(QuadIndices, sizeof(QuadIndices));
    s_Renderer2DData->QuadGraphicsPipeline = IGraphicsPipeline::Create({
        .Shader = s_Renderer2DData->QuadShader,
        .VertexBufferLayout = VertexBufferLayout,
        .DebugName = "Renderer2D - Quads"
    });
}

void CRenderer2D::Shutdown()
{
    delete s_Renderer2DData;
}
 
void CRenderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color)
{
    DrawQuad({ Position.x, Position.y, 0.0f }, Size, Color);
}

void CRenderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color)
{
    CRenderPacket RenderPacket;
    RenderPacket.Shader = s_Renderer2DData->QuadShader;
    RenderPacket.VertexBuffer = s_Renderer2DData->QuadVertexBuffer;
    RenderPacket.IndexBuffer = s_Renderer2DData->QuadIndexBuffer;
    RenderPacket.GraphicsPipeline = s_Renderer2DData->QuadGraphicsPipeline;

    const auto Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });

    RenderPacket.Shader->SetVector4f("u_Color", Color);
    
    CRenderer::DrawIndexed(RenderPacket, Transform);
}

void CRenderer2D::DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture)
{
    DrawQuad({ Position.x, Position.y, 0.0f }, Size, Texture);
}

void CRenderer2D::DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture)
{
    CRenderPacket RenderPacket;
    RenderPacket.Shader = s_Renderer2DData->QuadShader;
    RenderPacket.VertexBuffer = s_Renderer2DData->QuadVertexBuffer;
    RenderPacket.IndexBuffer = s_Renderer2DData->QuadIndexBuffer;
    RenderPacket.GraphicsPipeline = s_Renderer2DData->QuadGraphicsPipeline;
    RenderPacket.Texture = Texture;

    const auto Transform = glm::translate(glm::mat4(1.0f), Position) *
        glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });

    RenderPacket.Shader->SetVector4f("u_Color", glm::vec4(1.0f));
    
    CRenderer::DrawIndexed(RenderPacket, Transform);
}
