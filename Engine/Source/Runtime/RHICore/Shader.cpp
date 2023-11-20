#include "MoonlightPCH.h"
#include "Shader.h"
#include "OpenGLRHI/OpenGLShader.h"
#include "Renderer/Renderer.h"

std::shared_ptr<FShader> FShader::Create(const std::filesystem::path& Filepath)
{
    m_Name = Filepath.stem().string();
    
    switch (FRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<FOpenGLShader>(Filepath);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create shader! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create shader! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create shader! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create shader! Metal isn't supported!") return nullptr;
    }
    
    verifyEnginef(false, "Failed to create shader! Unknown/unsupported graphics API!")
    return nullptr;
}
