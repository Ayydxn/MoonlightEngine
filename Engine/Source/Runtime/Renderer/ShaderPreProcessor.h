#pragma once

#include "RHICore/Shader.h"

#include <unordered_map>

class FShaderPreProcessor
{
public:
    static std::unordered_map<EShaderStage, std::string> PreProcessShader(const std::string& ShaderSources);
private:
    static EShaderStage GetMoonlightShaderStageFromString(const std::string& ShaderStage);
};
