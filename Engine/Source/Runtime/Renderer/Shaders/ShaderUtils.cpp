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
