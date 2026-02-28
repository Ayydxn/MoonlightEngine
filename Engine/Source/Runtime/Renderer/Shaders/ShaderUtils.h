#pragma once

#include <slang.h>

#include <filesystem>

class CShaderUtils
{
public:
    static std::filesystem::path GetShaderStageCacheFileExtension(SlangStage ShaderStage);
};
