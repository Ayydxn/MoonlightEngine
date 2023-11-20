#pragma once

#include "RHICore/Shader.h"

#include <memory>

class FShaderLibrary
{
public:
    FShaderLibrary() = default;
    ~FShaderLibrary();

    static void Initialize();

    void Shutdown();

    void Add(const std::shared_ptr<FShader>& Shader);
    void Add(const std::string& Name, const std::shared_ptr<FShader>& Shader);
    void Load(const std::filesystem::path& Filepath);
    void Load(const std::string& Name, const std::filesystem::path& Filepath);

    static FShaderLibrary& GetInstance() { return *m_ShaderLibraryInstance; }

    std::shared_ptr<FShader> GetShader(const std::string& Name) const;

    std::unordered_map<std::string, std::shared_ptr<FShader>> GetShaders() { return m_Shaders; }
    std::unordered_map<std::string, std::shared_ptr<FShader>> GetShaders() const { return m_Shaders; }
private:
    inline static FShaderLibrary* m_ShaderLibraryInstance;
    inline static bool bIsInitialized = false;
    
    std::unordered_map<std::string, std::shared_ptr<FShader>> m_Shaders;
};
