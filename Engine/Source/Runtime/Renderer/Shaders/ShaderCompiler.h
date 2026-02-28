#pragma once

#include "RHICore/Shader.h"

#include <slang-com-ptr.h>
#include <slang.h>

struct CCompiledShaderStage
{
    SlangStage ShaderStage;
    std::string EntryPointName;
    std::vector<uint32> Bytecode;
};

class CShaderCompiler
{
public:
    CShaderCompiler() = default;
    ~CShaderCompiler() = default;
    
    void Initialize();
    void Shutdown();
    
    void CompileShader(const std::string& ShaderName, const std::string& ShaderSource, std::unordered_map<SlangStage, CCompiledShaderStage>& CompilationOutput);
    void CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, std::unordered_map<SlangStage, CCompiledShaderStage>& CompilationOutput);
private:
    void DiagnoseIfNeeded(slang::IBlob* DiagnosticsBlob);
private:
    Slang::ComPtr<slang::IGlobalSession> m_GlobalSession;
    Slang::ComPtr<slang::ISession> m_Session;
};
