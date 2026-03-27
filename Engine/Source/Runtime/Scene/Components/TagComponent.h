#pragma once

#include "CoreDefines.h"

#include <string>
#include <vector>

struct MOONLIGHT_API CTagComponent
{
    std::vector<std::string> Tags = {};

    CTagComponent() = default;
    CTagComponent(const CTagComponent&) = default;

    void AddTag(const std::string& NewTag)
    {
        Tags.push_back(NewTag);
    }

    void RemoveTag(const std::string& Tag)
    {
        std::erase(Tags, Tag);
    }

    bool HasTag(const std::string& Tag) const
    {
        return std::ranges::find(Tags, Tag) != Tags.end();
    }
};
