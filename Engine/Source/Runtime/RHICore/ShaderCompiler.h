#pragma once

#include "Core/CoreTypes.h"
#include "RHICore/Shader.h"

#include <filesystem>

class IShaderCompiler
{
public:
    virtual ~IShaderCompiler() = default;
    
    static void Init();

    virtual void Initialize() = 0;
    virtual void Destroy() = 0;

    virtual std::vector<uint32> CompileShader(const std::string& ShaderName, const std::string& ShaderSource, EShaderStage ShaderStage) = 0;
    virtual std::vector<uint32> CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, EShaderStage ShaderStage) = 0;

    static IShaderCompiler& GetInstance() { return *m_ShaderCompilerInstance; }
private:
    static std::shared_ptr<IShaderCompiler> CreateInstance();
private:
    inline static std::shared_ptr<IShaderCompiler> m_ShaderCompilerInstance = nullptr;
};
