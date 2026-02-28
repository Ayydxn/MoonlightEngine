#pragma once

#include "CoreDefines.h"

#include <glm/glm.hpp>

#include <filesystem>
#include <memory>
#include <string>

class MOONLIGHT_API IShader
{
public:
    virtual ~IShader() = default;

    const std::string& GetName() const { return m_Name; }

    virtual void SetInt(const std::string& Name, int32 Value) const = 0;
    virtual void SetIntArray(const std::string& Name, int32* Values, int32 ValueCount) const = 0;
    virtual void SetFloat(const std::string& Name, float Value) const = 0;
    virtual void SetVector4f(const std::string& Name, const glm::vec4& Value) const = 0;
    virtual void SetMatrix4x4f(const std::string& Name, const glm::mat4& Value) const = 0;

    static std::shared_ptr<IShader> Create(const std::string& Name, const std::string& Source);
    static std::shared_ptr<IShader> Create(const std::filesystem::path& Filepath);
protected:
    inline static std::string m_Name;
};
