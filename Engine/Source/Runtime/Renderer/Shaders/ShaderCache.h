#pragma once

#include "ShaderCompiler.h"

#include <slang.h>

#define CURRENT_SHADER_CACHE_VERSION 1

struct CShaderCacheHeader
{
    // Fixed identifier that says "This is a Moonlight Engine shader cache file". Should never be changed.
    uint32 MagicNumber = 0x6D6C7363;
    
    // Only bump this when a breaking change is made to either this struct or CShaderCacheStageHeader.
    // Breaking changes can be considered as things such as:
    // - Adding/removing fields
    // - Changing a field's type
    // - The ordering of fields
    // - How shader stage data is laid out in the cache file.
    // When bumping, change the number represented by the macro. Not the value of this.
    uint32 Version = CURRENT_SHADER_CACHE_VERSION;
    
    uint32 ShaderStageCount;
    uint64 SourceHash;
};

struct CShaderCacheStageHeader
{
    SlangStage Stage;
    uint32 EntryPointNameLength;
    uint32 BytecodeSize;
};

class CShaderCache
{
public:
    static bool TryLoad(const std::filesystem::path& CacheFilepath, uint64 SourceHash, std::unordered_map<SlangStage, CCompiledShaderStage>& Output);
    static void Write(const std::filesystem::path& CacheFilepath, uint64 SourceHash, const std::unordered_map<SlangStage, CCompiledShaderStage>& CompiledStages);
    static void DumpBytecode(const std::filesystem::path& CacheFilepath, const std::filesystem::path& OutputDirectory);

    static std::filesystem::path GetCacheFilepath(const std::string& ApplicationName, const std::string& ShaderName);
    static uint64 ComputeSourceHash(const std::string& ShaderSource);
};
