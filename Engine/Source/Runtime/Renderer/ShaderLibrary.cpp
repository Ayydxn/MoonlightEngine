#include "MoonlightPCH.h"
#include "ShaderLibrary.h"

FShaderLibrary::~FShaderLibrary()
{
    for (auto&& [ShaderName, Shader] : m_Shaders)
        Shader.reset();
}

void FShaderLibrary::Initialize()
{
    if (bIsInitialized)
    {
        ENGINE_LOG_WARN_TAG("Renderer", "The shader library cannot be initializd more than once!");
        return;
    }
    
    m_ShaderLibraryInstance = new FShaderLibrary();
}

void FShaderLibrary::Shutdown()
{
    for (auto&& [ShaderName, Shader] : m_Shaders)
        Shader.reset();

    delete m_ShaderLibraryInstance;
    m_ShaderLibraryInstance = nullptr;

    bIsInitialized = false;
}

void FShaderLibrary::Add(const std::shared_ptr<FShader>& Shader)
{
    const std::string& ShaderName = Shader->GetName();
    Add(ShaderName, Shader);
}

void FShaderLibrary::Add(const std::string& Name, const std::shared_ptr<FShader>& Shader)
{
    if (m_Shaders.contains(Name))
    {
        ENGINE_LOG_WARN_TAG("Renderer", "The shader '{}' is already present in the shader library! It can't be added!", Name);
        return;
    }

    m_Shaders[Name] = Shader;
}

void FShaderLibrary::Load(const std::filesystem::path& Filepath)
{
    const auto Shader = FShader::Create(Filepath);
    Add(Shader);
}

void FShaderLibrary::Load(const std::string& Name, const std::filesystem::path& Filepath)
{
    const auto Shader = FShader::Create(Filepath);
    Add(Name, Shader);
}

std::shared_ptr<FShader> FShaderLibrary::GetShader(const std::string& Name) const
{
    if (!m_Shaders.contains(Name))
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "The shader '{}' doesn't exist in the shader library!");
        return nullptr;
    }
    
    return m_Shaders.at(Name);
}
