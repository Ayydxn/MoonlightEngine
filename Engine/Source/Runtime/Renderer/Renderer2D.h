#pragma once

#include "CoreDefines.h"
#include "RHICore/GraphicsPipeline.h"
#include "RHICore/IndexBuffer.h"
#include "RHICore/Shader.h"
#include "RHICore/Texture.h"
#include "RHICore/UniformBuffer.h"
#include "RHICore/VertexBuffer.h"

#include <glm/glm.hpp>

struct CRenderer2DSpecification
{
    uint32 MaxQuadsPerDrawCall = 10000;
};

struct CRenderer2DStatistics
{
    uint32 DrawCalls = 0;
    uint32 QuadCount = 0;
    
    uint32 GetTotalVertexCount() const { return QuadCount * 4; }
    uint32 GetTotalIndexCount() const { return QuadCount * 6; }
};


class MOONLIGHT_API CRenderer2D
{
public:
    CRenderer2D(const CRenderer2DSpecification& Specification = CRenderer2DSpecification());
    virtual ~CRenderer2D();

    void Initialize();
    void Shutdown();

    void BeginFrame(const glm::mat4& ViewProjectionMatrix);
    void EndFrame();
    
    void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color);
    void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color);
    void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);

    void DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color);
    void DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color);
    void DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    void DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    
    void ResetStats();
    
    const CRenderer2DStatistics& GetStats() const { return m_Statistics; }
private:
    void Flush();
    void StartNewBatch();
private:
    struct CQuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoords;
        float TextureIndex;
        float TilingFactor;
    };
    
    struct CGlobalUniforms
    {
        glm::mat4 Transform;
        glm::mat4 ViewProjectionMatrix;
    };
    
    static constexpr uint32 MaxTextureSlots = 32;
    
    CRenderer2DSpecification m_Specification;

    /* -- Quads -- */
    std::shared_ptr<IShader> m_QuadShader;
    std::shared_ptr<IGraphicsPipeline> m_QuadGraphicsPipeline;
    std::shared_ptr<IVertexBuffer> m_QuadVertexBuffer;
    std::shared_ptr<IIndexBuffer> m_QuadIndexBuffer;
    CQuadVertex* m_QuadVertexBufferBase = nullptr;
    CQuadVertex* m_QuadVertexBufferPointer = nullptr;
    glm::vec4 m_QuadVertexPositions[4];
    uint32 m_QuadIndexCount = 0;
    
    /* -- Uniforms -- */
    std::shared_ptr<IUniformBuffer> m_GlobalUniformBuffer;

    /* -- Misc/State -- */
    glm::mat4 m_CurrentViewProjectionMatrix;
    std::array<std::shared_ptr<ITexture>, MaxTextureSlots> m_TextureSlots;
    std::shared_ptr<ITexture> m_WhiteTexture;
    CRenderer2DStatistics m_Statistics;
    uint32 m_MaxVertices;
    uint32 m_MaxIndices;
    uint32 m_TextureSlotIndex = 1; // Start at index 1 as index 0 is going to be our white texture.
};
