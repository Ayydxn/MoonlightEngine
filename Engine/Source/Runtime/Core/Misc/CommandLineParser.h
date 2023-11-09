#pragma once

#include "CommandLineArguments.h"
#include "Core/CoreTypes.h"

class FCommandLineParser
{
public:
    static bool Param(const FCommandLineArguments& CommandLineArguments, const std::string& Argument);
    
    static bool Value(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, int32* Value);
    static bool Value(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, float* Value);
    static bool Value(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, double* Value);
    static bool Value(const FCommandLineArguments& CommandLineArguments, const std::string& Argument, std::string* Value);
};
