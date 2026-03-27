#pragma once

#include "CoreDefines.h"

#include <string>

struct MOONLIGHT_API CNameComponent
{
    std::string Name;
    
    CNameComponent()
        : Name("Entity") {}
    CNameComponent(const CNameComponent&) = default;
    CNameComponent(const std::string& Name)
        : Name(Name) {}
};