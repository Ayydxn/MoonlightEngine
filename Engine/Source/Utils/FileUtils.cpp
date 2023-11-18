#include "MoonlightPCH.h"
#include "FileUtils.h"

std::string FFileUtils::ReadFile(const std::filesystem::path& Filepath)
{
    std::string Result;

    std::ifstream FileReader(Filepath, std::ios::in | std::ios::binary);
    if (FileReader)
    {
        FileReader.seekg(0, std::ios::end);

        const uint64 CurrentCharacterPosition = FileReader.tellg();
        if (CurrentCharacterPosition != static_cast<uint64>(-1))
        {
            Result.resize(CurrentCharacterPosition);
            FileReader.seekg(0, std::ios::beg);
            FileReader.read(Result.data(), static_cast<int64>(CurrentCharacterPosition));
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

    return Result;
}
