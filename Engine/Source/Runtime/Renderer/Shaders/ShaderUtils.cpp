#include "MoonlightPCH.h"
#include "ShaderUtils.h"

std::filesystem::path CShaderUtils::GetShaderStageCacheFileExtension(SlangStage ShaderStage)
{
    switch (ShaderStage)
    {
        // mlsc = Moonlight Shader Cache
        case SLANG_STAGE_NONE: return "UnknownCache.mlsc";
        case SLANG_STAGE_VERTEX: return "VertexCache.mlsc";
        case SLANG_STAGE_FRAGMENT: return "FragmentCache.mlsc";
        default:
        {
            verifyEnginef(false, "Failed to get shader cache file extension for shader stage!")
            return "UnknownCache.mlsc";
        }
    }
}
