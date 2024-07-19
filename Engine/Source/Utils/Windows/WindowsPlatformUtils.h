#pragma once

#include "Utils/PlatformUtils.h"

class CWindowsPlatformUtils : public IPlatformUtils
{
public:
    CWindowsPlatformUtils() = default;
    ~CWindowsPlatformUtils() override = default;

    std::filesystem::path GetApplicationDirectory_Implementation() override;
};
