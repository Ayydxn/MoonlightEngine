#include "MoonlightPCH.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanContext.h"
#include "VulkanShader.h"

namespace
{
    vk::PrimitiveTopology ConvertMoonlightTopologyToVulkan(const EPrimitiveTopology PrimitiveTopology)
    {
        switch (PrimitiveTopology)
        {
            case EPrimitiveTopology::None: return static_cast<vk::PrimitiveTopology>(VK_PRIMITIVE_TOPOLOGY_MAX_ENUM);
            case EPrimitiveTopology::Points: return vk::PrimitiveTopology::ePointList;
            case EPrimitiveTopology::Lines: return vk::PrimitiveTopology::eLineList;
            case EPrimitiveTopology::Triangles: return vk::PrimitiveTopology::eTriangleList;
            case EPrimitiveTopology::LineStrip: return vk::PrimitiveTopology::eLineStrip;
            case EPrimitiveTopology::TriangleStrip: return vk::PrimitiveTopology::eTriangleStrip;
            case EPrimitiveTopology::TriangleFan: return vk::PrimitiveTopology::eTriangleFan;
        }

        verifyEnginef(false, "Failed to convert Moonlight primitive topology to Vulkan!")
        return static_cast<vk::PrimitiveTopology>(VK_PRIMITIVE_TOPOLOGY_MAX_ENUM);
    }

    vk::CompareOp ConvertMoonlightCompareOpToVulkan(const EDepthCompareOperator DepthCompareOperator)
    {
        switch (DepthCompareOperator)
        {
            case EDepthCompareOperator::None: return static_cast<vk::CompareOp>(VK_COMPARE_OP_MAX_ENUM);
            case EDepthCompareOperator::Never: return vk::CompareOp::eNever;
            case EDepthCompareOperator::NotEqual: return vk::CompareOp::eNotEqual;
            case EDepthCompareOperator::Less: return vk::CompareOp::eLess;
            case EDepthCompareOperator::LessOrEqual: return vk::CompareOp::eLessOrEqual;
            case EDepthCompareOperator::Greater: return vk::CompareOp::eGreater;
            case EDepthCompareOperator::GreaterOrEqual: return vk::CompareOp::eGreaterOrEqual;
            case EDepthCompareOperator::Equal: return vk::CompareOp::eEqual;
            case EDepthCompareOperator::Always: return vk::CompareOp::eAlways;
        }

        verifyEnginef(false, "Failed to convert Moonlight primitive topology to Vulkan!")
        return static_cast<vk::CompareOp>(VK_COMPARE_OP_MAX_ENUM);
    }
}

CVulkanGraphicsPipeline::CVulkanGraphicsPipeline(const CGraphicsPipelineSpecification& Specification)
    : m_LogicalDevice(CVulkanContext::GetInstance()->GetLogicalDevice()->GetHandle()), m_PipelineSpecification(Specification)
{
    verifyEnginef(Specification.Shader, "A graphics pipeline requires a shader! It cannot be null!")

    if (m_PipelineSpecification.DebugName.empty())
        m_PipelineSpecification.DebugName = Specification.Shader->GetName();

    CreatePipelineLayoutAndCache();
    CVulkanGraphicsPipeline::Invalidate();
}

CVulkanGraphicsPipeline::~CVulkanGraphicsPipeline()
{
    m_LogicalDevice.destroyPipeline(m_GraphicsPipelineHandle);
    m_LogicalDevice.destroyPipelineLayout(m_GraphicsPipelineLayout);
    m_LogicalDevice.destroyPipelineCache(m_GraphcisPipelineCache);
}

void CVulkanGraphicsPipeline::Invalidate()
{
    ENGINE_LOG_DEBUG_TAG("Renderer", "Creating Vulkan graphics pipeline '{}'...", m_PipelineSpecification.DebugName);
    
    const auto& VulkanShader = std::dynamic_pointer_cast<CVulkanShader>(m_PipelineSpecification.Shader);
    
    const std::vector<vk::DynamicState> DynamicStates =
    {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
    };

    vk::Viewport Viewport;
    Viewport.x = 0.0f;
    Viewport.y = 0.0f;
    Viewport.width = static_cast<float>(CVulkanContext::GetInstance()->GetSwapChain()->GetExtent().width);
    Viewport.height = static_cast<float>(CVulkanContext::GetInstance()->GetSwapChain()->GetExtent().height);
    Viewport.minDepth = 0.0f;
    Viewport.maxDepth = 1.0f;

    vk::Rect2D ViewportScissor;
    ViewportScissor.offset = vk::Offset2D(0, 0);
    ViewportScissor.extent = CVulkanContext::GetInstance()->GetSwapChain()->GetExtent();

    vk::PipelineDynamicStateCreateInfo PipelineDynamicStateCreateInfo;
    PipelineDynamicStateCreateInfo.sType = vk::StructureType::ePipelineDynamicStateCreateInfo;
    PipelineDynamicStateCreateInfo.dynamicStateCount = static_cast<uint32>(DynamicStates.size());
    PipelineDynamicStateCreateInfo.pDynamicStates = DynamicStates.data();
    PipelineDynamicStateCreateInfo.flags = vk::PipelineDynamicStateCreateFlags();

    // TODO: (Ayydxn) Fill this out.
    vk::PipelineVertexInputStateCreateInfo PipelineVertexInputStateCreateInfo;
    PipelineVertexInputStateCreateInfo.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo;
    PipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
    PipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr;
    PipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
    PipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;
    PipelineVertexInputStateCreateInfo.flags = vk::PipelineVertexInputStateCreateFlags();

    vk::PipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo;
    PipelineInputAssemblyStateCreateInfo.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
    PipelineInputAssemblyStateCreateInfo.topology = ConvertMoonlightTopologyToVulkan(m_PipelineSpecification.Topology);
    PipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = false;
    PipelineInputAssemblyStateCreateInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();

    vk::PipelineViewportStateCreateInfo PipelineViewportStateCreateInfo;
    PipelineViewportStateCreateInfo.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
    PipelineViewportStateCreateInfo.viewportCount = 1;
    PipelineViewportStateCreateInfo.pViewports = &Viewport;
    PipelineViewportStateCreateInfo.scissorCount = 1;
    PipelineViewportStateCreateInfo.pScissors = &ViewportScissor;
    PipelineViewportStateCreateInfo.flags = vk::PipelineViewportStateCreateFlags();

    vk::PipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo;
    PipelineRasterizationStateCreateInfo.sType = vk::StructureType::ePipelineRasterizationStateCreateInfo;
    PipelineRasterizationStateCreateInfo.depthClampEnable = false;
    PipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = false; 
    PipelineRasterizationStateCreateInfo.polygonMode = m_PipelineSpecification.bEnableWireframe ? vk::PolygonMode::eLine : vk::PolygonMode::eFill;
    PipelineRasterizationStateCreateInfo.lineWidth = m_PipelineSpecification.LineWidth;
    PipelineRasterizationStateCreateInfo.cullMode = m_PipelineSpecification.bEnableBackfaceCulling ? vk::CullModeFlagBits::eBack : vk::CullModeFlagBits::eNone;
    PipelineRasterizationStateCreateInfo.frontFace = vk::FrontFace::eClockwise;
    PipelineRasterizationStateCreateInfo.depthBiasEnable = false;
    PipelineRasterizationStateCreateInfo.flags = vk::PipelineRasterizationStateCreateFlags();

    // TODO: (Ayydan) When we support MSAA, fill this out.
    vk::PipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo;
    PipelineMultisampleStateCreateInfo.sType = vk::StructureType::ePipelineMultisampleStateCreateInfo;
    PipelineMultisampleStateCreateInfo.sampleShadingEnable = false;
    PipelineMultisampleStateCreateInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
    PipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
    PipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
    PipelineMultisampleStateCreateInfo.alphaToCoverageEnable = false;
    PipelineMultisampleStateCreateInfo.alphaToOneEnable = false;
    PipelineMultisampleStateCreateInfo.flags = vk::PipelineMultisampleStateCreateFlags();

    vk::PipelineDepthStencilStateCreateInfo PipelineDepthStencilStateCreateInfo;
    PipelineDepthStencilStateCreateInfo.sType = vk::StructureType::ePipelineDepthStencilStateCreateInfo;
    PipelineDepthStencilStateCreateInfo.depthTestEnable = m_PipelineSpecification.bEnableDepthTesting;
    PipelineDepthStencilStateCreateInfo.depthWriteEnable = m_PipelineSpecification.bEnableDepthWriting;
    PipelineDepthStencilStateCreateInfo.depthCompareOp = ConvertMoonlightCompareOpToVulkan(m_PipelineSpecification.DepthCompareOperator);
    PipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = false;
    PipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
    PipelineDepthStencilStateCreateInfo.maxDepthBounds = 0.0f;
    PipelineDepthStencilStateCreateInfo.stencilTestEnable = false;
    PipelineDepthStencilStateCreateInfo.back.failOp = vk::StencilOp::eKeep;
    PipelineDepthStencilStateCreateInfo.back.passOp = vk::StencilOp::eKeep;
    PipelineDepthStencilStateCreateInfo.back.compareOp = vk::CompareOp::eAlways;
    PipelineDepthStencilStateCreateInfo.front = PipelineDepthStencilStateCreateInfo.back;
    PipelineDepthStencilStateCreateInfo.flags = vk::PipelineDepthStencilStateCreateFlags();

    vk::PipelineColorBlendAttachmentState PipelineColorBlendAttachmentState;
    PipelineColorBlendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    PipelineColorBlendAttachmentState.blendEnable = true;
    PipelineColorBlendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    PipelineColorBlendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    PipelineColorBlendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;
    PipelineColorBlendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    PipelineColorBlendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    PipelineColorBlendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
    
    vk::PipelineColorBlendStateCreateInfo PipelineColorBlendStateCreateInfo;
    PipelineColorBlendStateCreateInfo.sType = vk::StructureType::ePipelineColorBlendStateCreateInfo;
    PipelineColorBlendStateCreateInfo.attachmentCount = 1;
    PipelineColorBlendStateCreateInfo.pAttachments = &PipelineColorBlendAttachmentState;
    PipelineColorBlendStateCreateInfo.flags = vk::PipelineColorBlendStateCreateFlags();

    vk::PipelineRenderingCreateInfoKHR PipelineRenderingCreateInfo;
    PipelineRenderingCreateInfo.sType = vk::StructureType::ePipelineRenderingCreateInfoKHR;
    
    vk::GraphicsPipelineCreateInfo GraphicsPipelineCreateInfo;
    GraphicsPipelineCreateInfo.sType = vk::StructureType::eGraphicsPipelineCreateInfo;
    GraphicsPipelineCreateInfo.stageCount = static_cast<uint32>(VulkanShader->GetPipelineShaderStageCreateInfos().size());
    GraphicsPipelineCreateInfo.pStages = VulkanShader->GetPipelineShaderStageCreateInfos().data();
    GraphicsPipelineCreateInfo.pDynamicState = &PipelineDynamicStateCreateInfo;
    GraphicsPipelineCreateInfo.pVertexInputState = &PipelineVertexInputStateCreateInfo;
    GraphicsPipelineCreateInfo.pInputAssemblyState = &PipelineInputAssemblyStateCreateInfo;
    GraphicsPipelineCreateInfo.pViewportState = &PipelineViewportStateCreateInfo;
    GraphicsPipelineCreateInfo.pRasterizationState = &PipelineRasterizationStateCreateInfo;
    GraphicsPipelineCreateInfo.pMultisampleState = &PipelineMultisampleStateCreateInfo;
    GraphicsPipelineCreateInfo.pDepthStencilState = &PipelineDepthStencilStateCreateInfo;
    GraphicsPipelineCreateInfo.pColorBlendState = &PipelineColorBlendStateCreateInfo;
    GraphicsPipelineCreateInfo.layout = m_GraphicsPipelineLayout;
    GraphicsPipelineCreateInfo.renderPass = nullptr;
    GraphicsPipelineCreateInfo.basePipelineHandle = nullptr;
    GraphicsPipelineCreateInfo.basePipelineIndex = -1;
    GraphicsPipelineCreateInfo.pNext = &PipelineRenderingCreateInfo;
    
    try
    {
        m_GraphicsPipelineHandle = m_LogicalDevice.createGraphicsPipeline(m_GraphcisPipelineCache, GraphicsPipelineCreateInfo).value;
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to create Vulkan graphics pipeline!", VulkanSystemError.what())
    }
}

void CVulkanGraphicsPipeline::CreatePipelineLayoutAndCache()
{
    // TODO: (Ayydxn) Fill this out.
    vk::PipelineLayoutCreateInfo PipelineLayoutCreateInfo;
    PipelineLayoutCreateInfo.sType = vk::StructureType::ePipelineLayoutCreateInfo;
    PipelineLayoutCreateInfo.setLayoutCount = 0;
    PipelineLayoutCreateInfo.pSetLayouts = nullptr;
    PipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    PipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
    PipelineLayoutCreateInfo.flags = vk::PipelineLayoutCreateFlags();

    vk::PipelineCacheCreateInfo PipelineCacheCreateInfo;
    PipelineCacheCreateInfo.sType = vk::StructureType::ePipelineCacheCreateInfo;
    PipelineCacheCreateInfo.flags = vk::PipelineCacheCreateFlags();

    try
    {
        m_GraphicsPipelineLayout = m_LogicalDevice.createPipelineLayout(PipelineLayoutCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to create Vulkan pipeline layout! ({})", VulkanSystemError.what())
    }

    try
    {
        m_GraphcisPipelineCache = m_LogicalDevice.createPipelineCache(PipelineCacheCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to create Vulkan pipeline cache! ({})", VulkanSystemError.what())
    }
}
