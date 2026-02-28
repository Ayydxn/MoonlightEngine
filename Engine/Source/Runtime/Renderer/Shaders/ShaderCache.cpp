#include "MoonlightPCH.h"
#include "ShaderCache.h"
#include "Core/Misc/EnginePaths.h"

bool CShaderCache::TryLoad(const std::filesystem::path& CacheFilepath, uint64 SourceHash, std::unordered_map<SlangStage, CCompiledShaderStage>& Output)
{
    std::ifstream FileReader(CacheFilepath, std::ios::in | std::ios::binary);

    if (!FileReader.is_open())
        return false;

    CShaderCacheHeader Header;
    FileReader.read(reinterpret_cast<char*>(&Header), sizeof(CShaderCacheHeader));

    if (Header.MagicNumber != 0x6D6C7363 || Header.Version != CURRENT_SHADER_CACHE_VERSION)
    {
        ENGINE_LOG_WARN_TAG("Renderer", "Shader cache file '{}' is invalid or outdated! Recompiling...", CacheFilepath.string());
        return false;
    }

    if (Header.SourceHash != SourceHash)
    {
        ENGINE_LOG_INFO_TAG("Renderer", "Shader source has changed for '{}'! Recompiling...", CacheFilepath.string());
        return false;
    }

    ENGINE_LOG_INFO_TAG("Renderer", "Loading shader from cache '{}'...", CacheFilepath.string());

    for (uint32 i = 0; i < Header.ShaderStageCount; i++)
    {
        CShaderCacheStageHeader StageHeader;
        FileReader.read(reinterpret_cast<char*>(&StageHeader), sizeof(CShaderCacheStageHeader));

        std::string EntryPointName(StageHeader.EntryPointNameLength, '\0');
        FileReader.read(EntryPointName.data(), StageHeader.EntryPointNameLength);

        CCompiledShaderStage CompiledStage;
        CompiledStage.ShaderStage = StageHeader.Stage;
        CompiledStage.EntryPointName = EntryPointName;
        CompiledStage.Bytecode.resize(StageHeader.BytecodeSize / sizeof(uint32));
        
        FileReader.read(reinterpret_cast<char*>(CompiledStage.Bytecode.data()), StageHeader.BytecodeSize);

        Output[StageHeader.Stage] = std::move(CompiledStage);
    }

    return true;
}

void CShaderCache::Write(const std::filesystem::path& CacheFilepath, const uint64 SourceHash, const std::unordered_map<SlangStage, CCompiledShaderStage>& CompiledStages)
{
    std::ofstream FileWriter(CacheFilepath, std::ios::out | std::ios::binary);

    if (!FileWriter.is_open())
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to write shader cache file '{}'!", CacheFilepath.string());
        return;
    }

    ENGINE_LOG_INFO_TAG("Renderer", "Writing shader to cache '{}'...", CacheFilepath.string());

    CShaderCacheHeader Header;
    Header.ShaderStageCount = static_cast<uint32>(CompiledStages.size());
    Header.SourceHash = SourceHash;
    
    FileWriter.write(reinterpret_cast<const char*>(&Header), sizeof(CShaderCacheHeader));

    for (const auto& [Stage, CompiledStage] : CompiledStages)
    {
        CShaderCacheStageHeader StageHeader;
        StageHeader.Stage = Stage;
        StageHeader.EntryPointNameLength = static_cast<uint32>(CompiledStage.EntryPointName.size());
        StageHeader.BytecodeSize = static_cast<uint32>(CompiledStage.Bytecode.size() * sizeof(uint32));
        
        FileWriter.write(reinterpret_cast<const char*>(&StageHeader), sizeof(CShaderCacheStageHeader));
        FileWriter.write(CompiledStage.EntryPointName.data(), static_cast<int64>(CompiledStage.EntryPointName.size()));
        FileWriter.write(reinterpret_cast<const char*>(CompiledStage.Bytecode.data()), StageHeader.BytecodeSize);
    }

    FileWriter.flush();
    FileWriter.close();
}

void CShaderCache::DumpBytecode(const std::filesystem::path& CacheFilepath, const std::filesystem::path& OutputDirectory)
{
    std::unordered_map<SlangStage, CCompiledShaderStage> Stages;

    // Pass 0 as hash to bypass hash check - we just want the bytecode
    std::ifstream FileReader(CacheFilepath, std::ios::in | std::ios::binary);

    if (!FileReader.is_open())
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to open cache file '{}' for bytecode export!", CacheFilepath.string());
        return;
    }

    CShaderCacheHeader Header;
    FileReader.read(reinterpret_cast<char*>(&Header), sizeof(CShaderCacheHeader));

    if (Header.MagicNumber != 0x6D6C7363)
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Cache file '{}' is not a valid Moonlight shader cache file!", CacheFilepath.string());
        return;
    }

    for (uint32 i = 0; i < Header.ShaderStageCount; i++)
    {
        CShaderCacheStageHeader StageHeader;
        FileReader.read(reinterpret_cast<char*>(&StageHeader), sizeof(CShaderCacheStageHeader));

        std::string EntryPointName(StageHeader.EntryPointNameLength, '\0');
        FileReader.read(EntryPointName.data(), StageHeader.EntryPointNameLength);

        std::vector<uint32> Bytecode(StageHeader.BytecodeSize / sizeof(uint32));
        FileReader.read(reinterpret_cast<char*>(Bytecode.data()), StageHeader.BytecodeSize);

        const std::filesystem::path OutputFilepath = OutputDirectory / 
            (CacheFilepath.stem().string() + "_" + EntryPointName + ".spv");

        std::ofstream FileWriter(OutputFilepath, std::ios::out | std::ios::binary);

        if (FileWriter.is_open())
        {
            FileWriter.write(reinterpret_cast<const char*>(Bytecode.data()), StageHeader.BytecodeSize);
            FileWriter.flush();
            FileWriter.close();

            ENGINE_LOG_INFO_TAG("Renderer", "Exported SPIR-V for entry point '{}' to '{}'", EntryPointName, OutputFilepath.string());
        }
    }
}

std::filesystem::path CShaderCache::GetCacheFilepath(const std::string& ApplicationName, const std::string& ShaderName)
{
    return CEnginePaths::GetShaderCacheDirectory() / (ApplicationName + "_" + ShaderName + ".mlsc");
}

uint64 CShaderCache::ComputeSourceHash(const std::string& ShaderSource)
{
    return std::hash<std::string>{}(ShaderSource);
}
