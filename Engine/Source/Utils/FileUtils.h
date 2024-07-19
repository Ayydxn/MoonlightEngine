#pragma once

#include <filesystem>

class CFileUtils
{
public:
    static std::string ReadFile(const std::filesystem::path& Filepath);
private:
    static int SkipByteOrderMark(std::istream& FileReader);
};
