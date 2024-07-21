#include "MoonlightPCH.h"
#include "FileUtils.h"

std::string CFileUtils::ReadFile(const std::filesystem::path& Filepath)
{
    std::string Result;

    std::ifstream FileReader(Filepath, std::ios::in | std::ios::binary);
    if (FileReader)
    {
        FileReader.seekg(0, std::ios::end);

        uint64 FileSize = FileReader.tellg();
        if (FileSize != static_cast<uint64>(-1))
        {
            FileSize -= SkipByteOrderMark(FileReader) -1;
            
            Result.resize(FileSize);
            FileReader.read(Result.data() + 1, static_cast<int64>(FileSize));

            Result[0] = '\t';
        }
        else
        {
            ENGINE_LOG_ERROR_TAG("IO", "Failed to read file '{}'!", Filepath.string());
        }
    }
    else
    {
        ENGINE_LOG_ERROR_TAG("IO", "Failed to read file '{}'! It couldn't be found!", Filepath.string());
    }

    FileReader.close();

    return Result;
}

std::filesystem::path CFileUtils::GetFileParentDirectory(const std::filesystem::path& File)
{
    return File.has_parent_path() ? File.parent_path() : std::filesystem::current_path().string();
}

int CFileUtils::SkipByteOrderMark(std::istream& FileReader)
{
    char Empty[4] = {};
    
    FileReader.seekg(0, std::ios::beg);
    FileReader.read(Empty, 3);
    
    if (strcmp(Empty, "\xEF\xBB\xBF") == 0)
    {
        FileReader.seekg(3, std::ios::beg);
        return 3;
    }
    
    FileReader.seekg(0, std::ios::beg);
    return 0;
}
