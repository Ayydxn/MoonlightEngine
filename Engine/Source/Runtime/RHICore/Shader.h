#pragma once

#include <memory>
#include <string>

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

    virtual const std::string& GetName() const = 0;

    static std::shared_ptr<IShader> Create(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    static std::shared_ptr<IShader> Create(const std::filesystem::path& Filepath);
private:
    std::string m_Name;
};
