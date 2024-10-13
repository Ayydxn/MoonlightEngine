#include "MoonlightPCH.h"
#include "Shader.h"
#include "OpenGLRHI/OpenGLShader.h"
#include "Renderer/Renderer.h"

std::shared_ptr<IShader> IShader::Create(const std::string& Name, const std::string& Source, EShaderStage ShaderStage)
{
    m_Name = Name;
    
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLShader>(Name, Source, ShaderStage);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create shader! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create shader! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create shader! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create shader! Metal isn't supported!") return nullptr;
    }
    
    verifyEnginef(false, "Failed to create shader! Unknown/unsupported graphics API!")
    return nullptr;
}

std::shared_ptr<IShader> IShader::Create(const std::filesystem::path& Filepath)
{
    m_Name = Filepath.stem().string();
    
    const std::string VerifyMessage = std::string("When creating a shader via file, you should not specify a file extension, but only the shader's name!")
        .append(" For example:\nCorrect: MyShader\nIncorrect: MyShader.vert.glsl");

    verifyEnginef(Filepath.filename().string().find('.') == std::string::npos, VerifyMessage)
    
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLShader>(Filepath);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create shader! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create shader! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create shader! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create shader! Metal isn't supported!") return nullptr;
    }
    
    verifyEnginef(false, "Failed to create shader! Unknown/unsupported graphics API!")
    return nullptr;
}
