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
protected:
    std::string m_Name;
};
