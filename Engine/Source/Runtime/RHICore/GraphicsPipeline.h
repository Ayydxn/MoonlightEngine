#pragma once

#include "RHICore/Shader.h"

enum class EPrimitiveTopology
{
    None,

    Points,
    Lines,
    Triangles,
    LineStrip,
    TriangleStrip,
    TriangleFan
};

enum class EDepthCompareOperator
{
    None,
    
    Never,
    NotEqual,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    Always
};

struct CGraphicsPipelineSpecification
{
    std::shared_ptr<IShader> Shader;
    EPrimitiveTopology Topology = EPrimitiveTopology::Triangles;
    EDepthCompareOperator DepthCompareOperator = EDepthCompareOperator::GreaterOrEqual;
    float LineWidth = 1.0f;
    bool bEnableBackfaceCulling = true;
    bool bEnableWireframe = false;
    bool bEnableDepthTesting = true;
    bool bEnableDepthWriting = true;

    std::string DebugName;
};

class IGraphicsPipeline
{
public:
    virtual ~IGraphicsPipeline() = default;

    virtual void Invalidate() = 0;

    static std::shared_ptr<IGraphicsPipeline> Create(const CGraphicsPipelineSpecification& Specification);
};
