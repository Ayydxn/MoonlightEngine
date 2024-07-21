#include "MoonlightPCH.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "VulkanRHI/VulkanShader.h"

std::shared_ptr<IShader> IShader::Create(const std::string& Name, const std::string& Source, EShaderStage ShaderStage)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: verifyEnginef(false, "Failed to create shader! OpenGL isn't supported!") return nullptr;
        case EGraphicsAPI::Vulkan: return std::make_shared<CVulkanShader>(Name, Source, ShaderStage);
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create shader! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create shader! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create shader! Metal isn't supported!") return nullptr;
    }
    
    verifyEnginef(false, "Failed to create shader! Unknown/unsupported graphics API!")
    return nullptr;
}

std::shared_ptr<IShader> IShader::Create(const std::filesystem::path& Filepath)
{
    const std::string VerifyMessage = std::string("When creating a shader via file, you should not specify a file extension, but only the shader's name!")
        .append(" For example:\nCorrect: MyShader\nIncorrect: MyShader.vert.glsl");

    verifyEnginef(Filepath.filename().string().find('.') == std::string::npos, VerifyMessage)
    
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: verifyEnginef(false, "Failed to create shader! OpenGL isn't supported!") return nullptr;
        case EGraphicsAPI::Vulkan: return std::make_shared<CVulkanShader>(Filepath);
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create shader! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create shader! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create shader! Metal isn't supported!") return nullptr;
    }
    
    verifyEnginef(false, "Failed to create shader! Unknown/unsupported graphics API!")
    return nullptr;
}
