#pragma once

#include "Utils/PlatformUtils.h"

class CLinuxPlatformUtils : public IPlatformUtils
{
public:
    CLinuxPlatformUtils() = default;
    ~CLinuxPlatformUtils() override = default;

    std::filesystem::path GetApplicationDirectory_Implementation() override;
};
