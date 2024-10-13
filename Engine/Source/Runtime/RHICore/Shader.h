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

    const std::string& GetName() const { return m_Name; }

    static std::shared_ptr<IShader> Create(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    static std::shared_ptr<IShader> Create(const std::filesystem::path& Filepath);
protected:
    inline static std::string m_Name;
};
