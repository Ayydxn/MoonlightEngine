#include "MoonlightPCH.h"
#include "Texture.h"
#include "OpenGLRHI/OpenGLTexture.h"
#include "Renderer/Renderer.h"

std::shared_ptr<ITexture> ITexture::Create(const std::filesystem::path& Filepath)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLTexture>(Filepath);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create texture! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create texture! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create texture! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create texture! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create texture! Unknown/unsupported graphics API!")
    return nullptr;
}
