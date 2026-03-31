#pragma once

struct CFontConfiguration
{
    std::string Name;
    std::string Filepath;
    float Size;
};

class CImGuiFonts
{
public:
    static void Add(const CFontConfiguration& Configuration, bool bIsDefault = false);
    
    static void PushFont(const std::string& Name);
    static void PopFont();
};
