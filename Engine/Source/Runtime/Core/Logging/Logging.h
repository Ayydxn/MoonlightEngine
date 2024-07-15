#pragma once

#undef FMT_STRING
#undef FMT_STRING_IMPL

#include "Core/CoreTypes.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>

class MOONLIGHT_API spdlog::logger;

enum class ELogType : uint8
{
    Engine,
    Application
};

enum class ELogLevel : uint8
{
    Trace,
    Info,
    Debug,
    Warning,
    Error,
    Critical
};

class MOONLIGHT_API CLogging
{
public:
    static void Initialize();
    static void Shutdown();

    template <typename... Arguments>
    static void LogMessage(ELogType LogType, ELogLevel LogLevel, std::string_view Tag, std::string_view Message, Arguments&& ... Args);

    template <typename... Arguments>
    static void LogAssertMessage(ELogType LogType, std::string_view Prefix, std::string_view Message, Arguments&& ... Args);
private:
    static std::string GetDateAsString();
    static std::string GetBackupLogID();
private:
    inline static std::shared_ptr<spdlog::logger> m_EngineLogger;
    inline static std::shared_ptr<spdlog::logger> m_ApplicationLogger;

    inline static bool bIsInitialized = false;
};

template <typename... Arguments>
void CLogging::LogMessage(ELogType LogType, ELogLevel LogLevel, std::string_view Tag, std::string_view Message, Arguments&& ... Args)
{
    const auto Logger = LogType == ELogType::Engine ? m_EngineLogger : m_ApplicationLogger;
    const std::string LogString = Tag.empty() ? "{}{}" : "[{}] {}";

    switch (LogLevel)
    {
        case ELogLevel::Trace:
        {
            // (Ayydan) Spdlog uses the fmt library to format log messages.
            // Since Moonlight is built with C++20, fmt by default has compile-time checks for format strings enabled.
            // So we do this to bypass those compile-time checks.
            Logger->trace(fmt::runtime(LogString), Tag, fmt::vformat(Message, fmt::make_format_args(Args...)));
            break;
        }

        case ELogLevel::Info:
        {
            Logger->info(fmt::runtime(LogString), Tag, fmt::vformat(Message, fmt::make_format_args(Args...)));
            break;
        }

        case ELogLevel::Debug:
        {
            Logger->debug(fmt::runtime(LogString), Tag, fmt::vformat(Message, fmt::make_format_args(Args...)));
            break;
        }

        case ELogLevel::Warning:
        {
            Logger->warn(fmt::runtime(LogString), Tag, fmt::vformat(Message, fmt::make_format_args(Args...)));
            break;
        }

        case ELogLevel::Error:
        {
            Logger->error(fmt::runtime(LogString), Tag, fmt::vformat(Message, fmt::make_format_args(Args...)));
            break;
        }

        case ELogLevel::Critical:
        {
            Logger->critical(fmt::runtime(LogString), Tag, fmt::vformat(Message, fmt::make_format_args(Args...)));
            break;
        }
    }
}

template <typename... Arguments>
void CLogging::LogAssertMessage(ELogType LogType, std::string_view Prefix, std::string_view Message, Arguments&&... Args)
{
    const auto Logger = LogType == ELogType::Engine ? m_EngineLogger : m_ApplicationLogger;
    Logger->error("{}: {}", Prefix, fmt::vformat(Message, fmt::make_format_args(Args...)));
}

/*-----------------------------*/
/* -- Engine Logging Macros -- */
/*-----------------------------*/

#define ENGINE_LOG_TRACE_TAG(Tag, Message, ...)         CLogging::LogMessage(ELogType::Engine, ELogLevel::Trace, Tag, Message, ##__VA_ARGS__)
#define ENGINE_LOG_INFO_TAG(Tag, Message, ...)          CLogging::LogMessage(ELogType::Engine, ELogLevel::Info, Tag, Message, ##__VA_ARGS__)
#define ENGINE_LOG_DEBUG_TAG(Tag, Message, ...)         CLogging::LogMessage(ELogType::Engine, ELogLevel::Debug, Tag, Message, ##__VA_ARGS__)
#define ENGINE_LOG_WARN_TAG(Tag, Message, ...)          CLogging::LogMessage(ELogType::Engine, ELogLevel::Warning, Tag, Message, ##__VA_ARGS__)
#define ENGINE_LOG_ERROR_TAG(Tag, Message, ...)         CLogging::LogMessage(ELogType::Engine, ELogLevel::Error, Tag, Message, ##__VA_ARGS__)
#define ENGINE_LOG_CRITICAL_TAG(Tag, Message, ...)      CLogging::LogMessage(ELogType::Engine, ELogLevel::Critical, Tag, Message, ##__VA_ARGS__)

#define ENGINE_LOG_TRACE(Message, ...)                  CLogging::LogMessage(ELogType::Engine, ELogLevel::Trace, "", Message, ##__VA_ARGS__)
#define ENGINE_LOG_INFO(Message, ...)                   CLogging::LogMessage(ELogType::Engine, ELogLevel::Info,  "", Message, ##__VA_ARGS__)
#define ENGINE_LOG_DEBUG(Message, ...)                  CLogging::LogMessage(ELogType::Engine, ELogLevel::Debug, "", Message, ##__VA_ARGS__)
#define ENGINE_LOG_WARN(Message, ...)                   CLogging::LogMessage(ELogType::Engine, ELogLevel::Warning, "", Message, ##__VA_ARGS__)
#define ENGINE_LOG_ERROR(Message, ...)                  CLogging::LogMessage(ELogType::Engine, ELogLevel::Error, "", Message, ##__VA_ARGS__)
#define ENGINE_LOG_CRITICAL(Message, ...)               CLogging::LogMessage(ELogType::Engine, ELogLevel::Critical, "", Message, ##__VA_ARGS__)

/*----------------------------------*/              
/* -- Application Logging Macros -- */              
/*----------------------------------*/              

#define APPLICATION_LOG_TRACE_TAG(Tag, Message, ...)    CLogging::LogMessage(ELogType::Application, ELogLevel::Trace, Tag, Message, ##__VA_ARGS__)
#define APPLICATION_LOG_INFO_TAG(Tag, Message, ...)     CLogging::LogMessage(ELogType::Application, ELogLevel::Info, Tag, Message, ##__VA_ARGS__)
#define APPLICATION_LOG_DEBUG_TAG(Tag, Message, ...)    CLogging::LogMessage(ELogType::Application, ELogLevel::Debug, Tag, Message, ##__VA_ARGS__)
#define APPLICATION_LOG_WARN_TAG(Tag, Message, ...)     CLogging::LogMessage(ELogType::Application, ELogLevel::Warning, Tag, Message, ##__VA_ARGS__)
#define APPLICATION_LOG_ERROR_TAG(Tag, Message, ...)    CLogging::LogMessage(ELogType::Application, ELogLevel::Error, Tag, Message, ##__VA_ARGS__)
#define APPLICATION_LOG_CRITICAL_TAG(Tag, Message, ...) CLogging::LogMessage(ELogType::Application, ELogLevel::Critical, Tag, Message, ##__VA_ARGS__)

#define APPLICATION_LOG_TRACE(Message, ...)             CLogging::LogMessage(ELogType::Application, ELogLevel::Trace, "", Message, ##__VA_ARGS__)
#define APPLICATION_LOG_INFO(Message, ...)              CLogging::LogMessage(ELogType::Application, ELogLevel::Info,  "", Message, ##__VA_ARGS__)
#define APPLICATION_LOG_DEBUG(Message, ...)             CLogging::LogMessage(ELogType::Application, ELogLevel::Debug, "", Message, ##__VA_ARGS__)
#define APPLICATION_LOG_WARN(Message, ...)              CLogging::LogMessage(ELogType::Application, ELogLevel::Warning, "", Message, ##__VA_ARGS__)
#define APPLICATION_LOG_ERROR(Message, ...)             CLogging::LogMessage(ELogType::Application, ELogLevel::Error, "", Message, ##__VA_ARGS__)
#define APPLICATION_LOG_CRITICAL(Message, ...)          CLogging::LogMessage(ELogType::Application, ELogLevel::Critical, "", Message, ##__VA_ARGS__)