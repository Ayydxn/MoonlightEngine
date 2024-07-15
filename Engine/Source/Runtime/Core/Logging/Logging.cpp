#include "MoonlightPCH.h"
#include "Logging.h"
#include "Application/Application.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void CLogging::Initialize()
{
    if (bIsInitialized)
    {
        m_EngineLogger->warn("Logging has already been initialized! It cannot be initialized more than once!");
        return;
    }

    if (!std::filesystem::exists("Logs"))
        std::filesystem::create_directory("Logs");

    std::string ApplicationName = CApplication::GetInstance().GetSpecification().Name;

    std::string EngineLogFileName = "Logs/Moonlight Engine-latest.log";
    std::string ApplicationLogFileName = std::format("Logs/{}-latest.log", ApplicationName);

    // If the "latest" log file exists, we rename it. This allows for the backup of older log files.
    // The same thing is done with the application's log file.
    if (std::filesystem::exists(EngineLogFileName))
    {
        std::stringstream StringStream;
        StringStream << std::format("Logs/Moonlight Engine-backup-{}-{}.log", GetDateAsString(), GetBackupLogID());

        std::filesystem::rename(EngineLogFileName, StringStream.str());
    }

    if (std::filesystem::exists(ApplicationLogFileName))
    {
        std::stringstream StringStream;
        StringStream << std::format("Logs/{}-backup-{}-{}.log", ApplicationName, GetDateAsString(), GetBackupLogID());

        std::filesystem::rename(ApplicationLogFileName, StringStream.str());
    }

    std::vector<spdlog::sink_ptr> EngineLoggerSinks =
    {
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(EngineLogFileName, true),

        // The console is disabled in Distribution builds, so we only create a console logger if we aren't building in Distribution.
        #ifndef MOONLIGHT_BUILD_DISTRIBUTION
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
        #endif
    };

    std::vector<spdlog::sink_ptr> ApplicationLoggerSinks =
    {
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(ApplicationLogFileName, true),

        #ifndef MOONLIGHT_BUILD_DISTRIBUTION
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
        #endif
    };

    EngineLoggerSinks[0]->set_pattern("[%I:%M:%S] [%l] (%n): %v");
    ApplicationLoggerSinks[0]->set_pattern("[%I:%M:%S] [%l] (%n): %v");

    #ifndef MOONLIGHT_BUILD_DISTRIBUTION
        EngineLoggerSinks[1]->set_pattern("[%I:%M:%S] %^[%l] (%n): %v%$");
        ApplicationLoggerSinks[1]->set_pattern("[%I:%M:%S] %^[%l] (%n): %v%$");
    #endif

    m_EngineLogger = std::make_shared<spdlog::logger>("Moonlight Engine", EngineLoggerSinks.begin(), EngineLoggerSinks.end());
    m_EngineLogger->set_level(spdlog::level::trace);

    m_ApplicationLogger = std::make_shared<spdlog::logger>(ApplicationName, ApplicationLoggerSinks.begin(), ApplicationLoggerSinks.end());
    m_ApplicationLogger->set_level(spdlog::level::trace);

    bIsInitialized = true;
}

void CLogging::Shutdown()
{
    m_EngineLogger.reset();
    m_ApplicationLogger.reset();

    spdlog::drop_all();

    bIsInitialized = false;
}

std::string CLogging::GetDateAsString()
{
    const auto TimeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::tm Time = {};
    assertEngine(localtime_s(&Time, &TimeNow) == 0);

    std::string DayString = std::to_string(Time.tm_mday);
    std::string MonthString = std::to_string(Time.tm_mon + 1);
    std::string YearString = std::to_string(Time.tm_year + 1900);

    return std::format("{}.{}.{}", MonthString, DayString, YearString);
}

std::string CLogging::GetBackupLogID()
{
    std::default_random_engine RandomEngine(static_cast<uint32>(std::time(nullptr)));
    std::uniform_int_distribution Distribution(10, 100);

    std::stringstream StringStream;
    StringStream << std::format("{}.{}.{}", Distribution(RandomEngine), Distribution(RandomEngine), Distribution(RandomEngine));

    return StringStream.str();
}
