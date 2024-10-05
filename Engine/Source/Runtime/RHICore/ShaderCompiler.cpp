#include "MoonlightPCH.h"
#include "ShaderCompiler.h"
#include "Renderer/Renderer.h"

void IShaderCompiler::Init()
{
    if (m_ShaderCompilerInstance != nullptr)
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Moonlight's shader compiler can only be initialized once!");
        return;
    }

    ENGINE_LOG_INFO_TAG("Renderer", "Initializing shader compiler...");

    m_ShaderCompilerInstance = CreateInstance();
    m_ShaderCompilerInstance->Initialize();
}

std::shared_ptr<IShaderCompiler> IShaderCompiler::CreateInstance()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: verifyEnginef(false, "Failed to create shader compiler! OpenGL isn't supported!") return nullptr;
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create shader compiler! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create shader compiler! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create shader compiler! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create shader compiler! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create shader compiler! Unknown/unsupported graphics API!")
    return nullptr;
}
