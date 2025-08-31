#pragma once

#include "CoreDefines.h"
#include "RHICore/GraphicsPipeline.h"
#include "RHICore/IndexBuffer.h"
#include "RHICore/Shader.h"
#include "RHICore/Texture.h"
#include "RHICore/VertexBuffer.h"

#include <glm/glm.hpp>

struct CRenderer2DSpecification
{
    uint32 MaxQuadsPerDrawCall = 10000;
};

class MOONLIGHT_API CRenderer2D
{
public:
    CRenderer2D(const CRenderer2DSpecification& Specification = CRenderer2DSpecification());
    virtual ~CRenderer2D();

    void Initialize();
    void Shutdown();

    void BeginFrame();
    void EndFrame();
    
    void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color);
    void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color);
    void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);

    void DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color);
    void DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color);
    void DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    void DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
private:
    void Flush();
private:
    struct CQuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoords;
    };
    
    CRenderer2DSpecification m_Specification;

    /* -- Quads -- */
    std::shared_ptr<IShader> m_QuadShader;
    std::shared_ptr<IGraphicsPipeline> m_QuadGraphicsPipeline;
    std::shared_ptr<IVertexBuffer> m_QuadVertexBuffer;
    std::shared_ptr<IIndexBuffer> m_QuadIndexBuffer;
    std::shared_ptr<ITexture> m_WhiteTexture;
    CQuadVertex* m_QuadVertexBufferBase = nullptr;
    CQuadVertex* m_QuadVertexBufferPointer = nullptr;
    uint32 m_QuadIndexCount = 0;

    uint32 m_MaxVertices;
    uint32 m_MaxIndices;
};
