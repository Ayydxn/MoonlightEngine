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

    const std::string& GetName() const { return m_Name; }
    
    static std::shared_ptr<FShader> Create(const std::filesystem::path& Filepath);
private:
    inline static std::string m_Name;
};
