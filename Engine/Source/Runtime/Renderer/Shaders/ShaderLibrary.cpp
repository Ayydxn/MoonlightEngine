#include "MoonlightPCH.h"
#include "ShaderLibrary.h"

void CShaderLibrary::Add(const std::shared_ptr<IShader>& Shader)
{
    const std::string& Name = Shader->GetName();
    if (m_Shaders.contains(Name))
    {
        ENGINE_LOG_WARN_TAG("Renderer", "Shader '{}' is already present in the shader library", Name);
        return;
    }

    m_Shaders[Name] = Shader;
}

void CShaderLibrary::Load(const std::filesystem::path& Filepath)
{
    const auto& Shader = IShader::Create(Filepath.string() + ".slang");

    m_Shaders[Shader->GetName()] = Shader;
}

void CShaderLibrary::Load(const std::string& Name, const std::filesystem::path& Filepath)
{
    const auto& Shader = IShader::Create(Filepath.string() + ".slang");

    m_Shaders[Name] = Shader;
}

const std::shared_ptr<IShader>& CShaderLibrary::GetShader(const std::string& Name) const
{
    verifyEnginef(m_Shaders.contains(Name), "Shader '{}' wasn't found in the shader library!", Name)

    return m_Shaders.at(Name);
}
