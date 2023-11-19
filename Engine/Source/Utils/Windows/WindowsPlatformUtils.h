#pragma once

#include "Utils/PlatformUtils.h"

class FWindowsPlatformUtils : public FPlatformUtils
{
public:
    FWindowsPlatformUtils() = default;
    ~FWindowsPlatformUtils() override = default;

    std::filesystem::path GetApplicationDirectory_Implementation() override;
};
