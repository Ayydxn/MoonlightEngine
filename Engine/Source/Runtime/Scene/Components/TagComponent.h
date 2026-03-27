#pragma once

#include "CoreDefines.h"

#include <string>

struct MOONLIGHT_API CTagComponent
{
    std::string Tag;
    
    CTagComponent() = default;
    CTagComponent(const CTagComponent&) = default;
    CTagComponent(const std::string& Tag)
        : Tag(Tag) {}
};