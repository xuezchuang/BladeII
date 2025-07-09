#pragma once

#include "Config.h"

#define B2NETWORK_LOG_LEVEL_TRACE 1
#define B2NETWORK_LOG_LEVEL_DEBUG 2
#define B2NETWORK_LOG_LEVEL_INFO  3
#define B2NETWORK_LOG_LEVEL_WARN  4
#define B2NETWORK_LOG_LEVEL_ERROR 5
#define B2NETWORK_LOG_LEVEL_FATAL 6

#ifndef B2NETWORK_LOG_LEVEL
#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA  // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
	//#define B2NETWORK_LOG_LEVEL B2NETWORK_LOG_LEVEL_TRACE
	//#define B2NETWORK_LOG_LEVEL B2NETWORK_LOG_LEVEL_DEBUG
	#define B2NETWORK_LOG_LEVEL B2NETWORK_LOG_LEVEL_INFO
	//#define B2NETWORK_LOG_LEVEL B2NETWORK_LOG_LEVEL_ERROR
	//#define B2NETWORK_LOG_LEVEL B2NETWORK_LOG_LEVEL_FATAL
#else
	#define B2NETWORK_LOG_LEVEL B2NETWORK_LOG_LEVEL_FATAL
#endif
#endif

#if B2NETWORK_LOG_LEVEL > B2NETWORK_LOG_LEVEL_TRACE
#define B2NETWORK_LOG_TRACE(Logger, Format, ...)
#else
#define B2NETWORK_LOG_TRACE(Logger, Format, ...) \
    { \
        Logger.Trace(Format, ##__VA_ARGS__); \
    }
#endif

#if B2NETWORK_LOG_LEVEL > B2NETWORK_LOG_LEVEL_DEBUG
#define B2NETWORK_LOG_DEBUG(Logger, Format, ...)
#else
#define B2NETWORK_LOG_DEBUG(Logger, Format, ...) \
    { \
        Logger.Debug(Format, ##__VA_ARGS__); \
    }
#endif

#if B2NETWORK_LOG_LEVEL > B2NETWORK_LOG_LEVEL_INFO
#define B2NETWORK_LOG_INFO(Logger, Format, ...)
#else
#define B2NETWORK_LOG_INFO(Logger, Format, ...) \
    { \
        Logger.Info(Format, ##__VA_ARGS__); \
    }
#endif

#if B2NETWORK_LOG_LEVEL > B2NETWORK_LOG_LEVEL_ERROR
#define B2NETWORK_LOG_ERROR(Logger, Format, ...)
#else
#define B2NETWORK_LOG_ERROR(Logger, Format, ...) \
    { \
        Logger.Error(Format, ##__VA_ARGS__); \
    }
#endif

namespace b2network {

class Logger
{
    static int s_tick;

    std::string m_name;

public:
    Logger(const std::string name);

    void Tick();

    bool IsDebugEnabled() const;

    void Trace(const char* format, ...);
    void Debug(const char* format, ...);
    void Info(const char* format, ...);
    void Error(const char* format, ...);

private:
	void Logging(ELogVerbosity::Type verb, const char* level, const char* msg);
    static FString GetLogTime();
};

class LogManager
{
    typedef std::shared_ptr<b2network::Logger> LoggerPtr;

public:
    static Logger& GetLogger(const std::string name);

private:
};

}
