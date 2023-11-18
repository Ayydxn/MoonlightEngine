#pragma once

#include <filesystem>
#include <memory>

enum class EShaderStage
{
    Unknown,

    Vertex,
    Fragment
};

class FShader
{
public:
    virtual ~FShader() = default;

    virtual void Bind() const = 0;

    static std::shared_ptr<FShader> Create(const std::filesystem::path& Filepath);
};
