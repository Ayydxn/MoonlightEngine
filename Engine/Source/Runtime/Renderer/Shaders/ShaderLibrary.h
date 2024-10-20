#pragma once

#include "CoreDefines.h"
#include "RHICore/Shader.h"

class MOONLIGHT_API CShaderLibrary
{
public:
    CShaderLibrary() = default;
    ~CShaderLibrary() = default;

    void Add(const std::shared_ptr<IShader>& Shader);
    
    void Load(const std::filesystem::path& Filepath);
    void Load(const std::string& Name, const std::filesystem::path& Filepath);

    const std::shared_ptr<IShader>& GetShader(const std::string& Name) const;
private:
    std::unordered_map<std::string, std::shared_ptr<IShader>> m_Shaders;
};
