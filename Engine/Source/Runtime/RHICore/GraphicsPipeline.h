#pragma once

#include "Renderer/VertexBufferLayout.h"

#include <memory>

struct FGraphicsPipelineSpecification
{
    FVertexBufferLayout VertexBufferLayout;
};

class FGraphicsPipeline
{
public:
    virtual ~FGraphicsPipeline() = default;

    virtual void Invalidate() = 0;
    virtual void Bind() = 0;

    static std::shared_ptr<FGraphicsPipeline> Create(const FGraphicsPipelineSpecification& GraphicsPipelineSpecification);
protected:
    inline static FGraphicsPipelineSpecification m_Specification;
};
