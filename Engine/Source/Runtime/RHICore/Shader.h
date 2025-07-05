#pragma once

#include "CoreDefines.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>

enum class EShaderStage
{
    Unknown,

    Vertex,
    Fragment
};

class MOONLIGHT_API IShader
{
public:
    virtual ~IShader() = default;

    const std::string& GetName() const { return m_Name; }

    virtual void SetInt(const std::string& Name, int32 Value) const = 0;
    virtual void SetFloat(const std::string& Name, float Value) const = 0;
    virtual void SetVector4f(const std::string& Name, const glm::vec4& Value) const = 0;
    virtual void SetMatrix4x4f(const std::string& Name, const glm::mat4& Value) const = 0;

    static std::shared_ptr<IShader> Create(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    static std::shared_ptr<IShader> Create(const std::filesystem::path& Filepath);
protected:
    inline static std::string m_Name;
};
