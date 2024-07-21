#include "MoonlightPCH.h"
#include "ShaderUtils.h"

std::filesystem::path CShaderUtils::GetShaderStageCacheFileExtension(const EShaderStage ShaderStage)
{
    switch (ShaderStage)
    {
        // mlsc = Moonlight Shader Cache
        case EShaderStage::Unknown: return "UnknownCache.mlsc";
        case EShaderStage::Vertex: return "VertexCache.mlsc";
        case EShaderStage::Fragment: return "FragmentCache.mlsc";
    }

    verifyEnginef(false, "Failed to get shader cache file extension for shader stage!")
    return "UnknownShaderStage.msc";
}

EShaderStage CShaderUtils::GetShaderStageFromFileExtension(const std::filesystem::path& ShaderFilename)
{
    const auto FirstDot = ShaderFilename.filename().string().find_first_of('.');
    const auto LastDot = ShaderFilename.filename().string().find_last_of('.');
    const std::string ShaderStageString = ShaderFilename.filename().string().substr(FirstDot + 1, LastDot - FirstDot - 1);

    if (strcmp(ShaderStageString.c_str(), "vert") == 0)
        return EShaderStage::Vertex;
    
    if (strcmp(ShaderStageString.c_str(), "frag") == 0)
        return EShaderStage::Fragment;
    
    return EShaderStage::Unknown;
}
