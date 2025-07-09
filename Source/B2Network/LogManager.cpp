//#include "B2NetworkPlugin.h"

#include "Config.h"
#include "LogManager.h"

#define ENABLE_B2NETWORK_LOGGING 1

using namespace b2network;

DEFINE_LOG_CATEGORY(LogB2Network);

int Logger::s_tick = 0;

Logger::Logger(const std::string name)
{
    m_name = "b2.net." + name;
}

void Logger::Tick()
{
    s_tick++;
}

bool Logger::IsDebugEnabled() const
{
#if B2NETWORK_LOG_LEVEL <= B2NETWORK_LOG_LEVEL_DEBUG
    return true;
#else
    return false;
#endif
}

FString Logger::GetLogTime()
{
    const int MAX_SIZE = 80;
    char buffer[MAX_SIZE];

    //const auto now = std::chrono::system_clock::now();
    //std::time_t t = std::chrono::system_clock::to_time_t(now);
    //struct tm* timeinfo = std::localtime(&t);
    //std::strftime(buffer, MAX_SIZE, "%F %X", timeinfo);

    return FString(buffer);
}

void Logger::Logging(ELogVerbosity::Type verb, const char* level, const char* msg)
{
#if !UE_BUILD_SHIPPING
	if (!IsRunningGame())
	{
		switch (verb)
		{
		case ELogVerbosity::Display:
			UE_LOG(LogB2Network, Display, TEXT("[%.8d] %s(%.8d) %s %s - %s"),
				FPlatformTLS::GetCurrentThreadId(), *GetLogTime(), s_tick, *FString(level), *FString(m_name.c_str()), *FString(msg));
			break;

		case ELogVerbosity::Warning:
			UE_LOG(LogB2Network, Warning, TEXT("[%.8d] %s(%.8d) %s %s - %s"),
				FPlatformTLS::GetCurrentThreadId(), *GetLogTime(), s_tick, *FString(level), *FString(m_name.c_str()), *FString(msg));
			break;

		default:
			UE_LOG(LogB2Network, Log, TEXT("[%.8d] %s(%.8d) %s %s - %s"),
				FPlatformTLS::GetCurrentThreadId(), *GetLogTime(), s_tick, *FString(level), *FString(m_name.c_str()), *FString(msg));
			break;
		}
	}
	else
	{
		switch (verb)
		{
		case ELogVerbosity::Display:
			UE_LOG(LogB2Network, Display, TEXT("%s %s - %s"),
				*FString(level), *FString(m_name.c_str()), *FString(msg));
			break;

		case ELogVerbosity::Warning:
			UE_LOG(LogB2Network, Warning, TEXT("%s %s - %s"),
				*FString(level), *FString(m_name.c_str()), *FString(msg));
			break;

		default:
			UE_LOG(LogB2Network, Log, TEXT("%s %s - %s"),
				*FString(level), *FString(m_name.c_str()), *FString(msg));
			break;
		}
	}
#elif FORCE_LOGGING_IN_ANDROID && PLATFORM_ANDROID
	if (!IsRunningGame())
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("[%.8d] %s(%.8d) %s %s - %s"),
			FPlatformTLS::GetCurrentThreadId(), *GetLogTime(), s_tick, *FString(level), *FString(m_name.c_str()), *FString(msg));
	}
	else
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("%s %s - %s"),
			*FString(level), *FString(m_name.c_str()), *FString(msg));
	}
#else
#endif
}

void Logger::Trace(const char* format, ...)
{
    const int MAX_SIZE = 4096;
    char buffer[MAX_SIZE];

    va_list args;
    va_start(args, format);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    vsnprintf_s(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_IOS)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#else
#error Platform not supported
#endif

    va_end(args);
	
	Logging(ELogVerbosity::Log, "TRACE", buffer);
}

void Logger::Debug(const char* format, ...)
{
    const int MAX_SIZE = 4096;
    char buffer[MAX_SIZE];

    va_list args;
    va_start(args, format);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    vsnprintf_s(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_IOS)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#else
#error Platform not supported
#endif
    
    va_end(args);

	Logging(ELogVerbosity::Log, "DEBUG", buffer);
}

void Logger::Info(const char* format, ...)
{
    const int MAX_SIZE = 4096;
    char buffer[MAX_SIZE];

    va_list args;
    va_start(args, format);
#if defined(B2NETWORK_PLATFORM_WINDOWS)
    vsnprintf_s(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_IOS)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#else
#error Platform not supported
#endif
    va_end(args);

	Logging(ELogVerbosity::Display, "INFO", buffer);
}

void Logger::Error(const char* format, ...)
{
    const int MAX_SIZE = 4096;
    char buffer[MAX_SIZE];

    va_list args;
    va_start(args, format);
#if defined(B2NETWORK_PLATFORM_WINDOWS)
    vsnprintf_s(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#elif defined(B2NETWORK_PLATFORM_IOS)
    vsnprintf(buffer, MAX_SIZE - 1, format, args);
#else
#error Platform not supported
#endif

    va_end(args);

	Logging(ELogVerbosity::Warning, "ERROR", buffer);
}

Logger& LogManager::GetLogger(const std::string name)
{
    static std::unordered_map<std::string, LoggerPtr> loggerMap;

    const auto it = loggerMap.find(name);
    if (it != loggerMap.end())
    {
        return *it->second;
    }
    else
    {
        auto logger = std::make_shared<Logger>(name);
        loggerMap.insert(std::make_pair(name, logger));
        return *logger;
    }
}
