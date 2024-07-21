#pragma once

#include <filesystem>

class CFileUtils
{
public:
    static std::string ReadFile(const std::filesystem::path& Filepath);

    static std::filesystem::path GetFileParentDirectory(const std::filesystem::path& File);
private:
    static int SkipByteOrderMark(std::istream& FileReader);
};
