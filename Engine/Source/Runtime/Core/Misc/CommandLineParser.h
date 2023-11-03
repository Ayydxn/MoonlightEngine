#pragma once

#include "CommandLineArguments.h"
#include "Core/CoreTypes.h"

class FCommandLineParser
{
public:
    static bool Param(const FCommandLineArguments& CommandLineArguments, const std::string& Argument);
    
    static bool Parse(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, int32* Value);
    static bool Parse(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, float* Value);
    static bool Parse(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, double* Value);
    static bool Parse(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, std::string* Value);
};
