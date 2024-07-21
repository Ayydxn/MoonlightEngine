#pragma once

#include <memory>

enum class EShaderStage
{
    Unknown,

    Vertex,
    Fragment
};

class IShader
{
public:
    virtual ~IShader() = default;

    static std::shared_ptr<IShader> Create(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    static std::shared_ptr<IShader> Create(const std::filesystem::path& Filepath);
};
