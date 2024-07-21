#pragma once

#include "RHICore/Shader.h"

#include <filesystem>

class CShaderUtils
{
public:
    static std::filesystem::path GetShaderStageCacheFileExtension(const EShaderStage ShaderStage);
    static EShaderStage GetShaderStageFromFileExtension(const std::filesystem::path& ShaderFilename);
};
