#pragma once

class FPlatformUtils
{
public:
    virtual ~FPlatformUtils() = default;

    static std::filesystem::path GetApplicationDirectory(); 
protected:
    virtual std::filesystem::path GetApplicationDirectory_Implementation() = 0;
private:
    static std::shared_ptr<FPlatformUtils> CreatePlatformUtilsInstance();
private:
    inline static std::shared_ptr<FPlatformUtils> m_PlatformUtilsInstance = CreatePlatformUtilsInstance();
};
