#pragma once

class IPlatformUtils
{
public:
    virtual ~IPlatformUtils() = default;

    static std::filesystem::path GetApplicationDirectory(); 
protected:
    virtual std::filesystem::path GetApplicationDirectory_Implementation() = 0;
private:
    static std::shared_ptr<IPlatformUtils> CreatePlatformUtilsInstance();
private:
    inline static std::shared_ptr<IPlatformUtils> m_PlatformUtilsInstance = CreatePlatformUtilsInstance();
};
