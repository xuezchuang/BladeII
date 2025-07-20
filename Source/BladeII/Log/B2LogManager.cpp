// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LogManager.h"
//#include "BladeII.h"

#include "algorithm"

#if defined(PLATFORM_WINDOWS)
#elif defined(PLATFORM_ANDROID)
#include <time.h>
#elif defined(PLATFORM_IOS)
#import <mach/mach.h>
#import <mach/mach_time.h>
#endif

#define DEBUG_LOG 0

#if DEBUG_LOG

using namespace B2Logger;

DEFINE_LOG_CATEGORY(BIILogManager);

#if UE_BUILD_SHIPPING
uint8 GB2LOGMANAGER_LOG_LEVEL = B2ClientLogLevel::B2_LOG_LEVEL_NOSHOW;
bool GB2LOGMANAGER_LOG_SHOW_FILEINFO = false;
#elif UE_BUILD_DEVELOPMENT
uint8 GB2LOGMANAGER_LOG_LEVEL = B2ClientLogLevel::B2_LOG_LEVEL_SHOWALL;
bool GB2LOGMANAGER_LOG_SHOW_FILEINFO = false;
#elif UE_BUILD_DEBUG
uint8 GB2LOGMANAGER_LOG_LEVEL = B2ClientLogLevel::B2_LOG_LEVEL_SHOWALL;
bool GB2LOGMANAGER_LOG_SHOW_FILEINFO = false;
#endif


#if PLATFORM_WINDOWS
#define VSPRINTF(buffer, size, format, args) vsnprintf_s(buffer, size, format, args);

#elif PLATFORM_ANDROID
#define VSPRINTF(buffer, size, format, args) vsnprintf(buffer, size, format, args);

const uint32 MILLI_IN_SECOND = 1000;

static long GetTickCount()
{
	struct timespec res;
	clock_gettime(CLOCK_MONOTONIC, &res);
	return (res.tv_sec * MILLI_IN_SECOND) + res.tv_nsec;
}
#elif PLATFORM_IOS
#define VSPRINTF(buffer, size, format, args) vsnprintf(buffer, size, format, args);

static long GetTickCount()
{
	return mach_absolute_time();
}
#else
#error Platform not supported
#endif

long Logger::m_startTick = GetTickCount();
#define LogElapsed static_cast<uint32>(GetTickCount() - m_startTick)

std::shared_ptr<B2LogFilterManager> B2LogFilterManager::m_pInstance = nullptr;
#define LOGFILTERMGR B2LogFilterManager::GetInstance()


Logger::Logger(const std::string _name) : m_name(_name)
{
	std::transform(_name.cbegin(), _name.cend(), m_namelower.begin(), ::tolower); // 家巩磊肺 傈券
}

void Logger::Trace(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...)
{
	if (GB2LOGMANAGER_LOG_LEVEL >= B2ClientLogLevel::B2_LOG_LEVEL_TRACE) return;
	if (LOGFILTERMGR->IsInFilter(m_namelower)) return;

	const int MAX_SIZE = 4096;
	char buffer[MAX_SIZE];

	va_list args;
	va_start(args, format);
	VSPRINTF(buffer, MAX_SIZE - 1, format, args);
	va_end(args);

	if(GB2LOGMANAGER_LOG_SHOW_FILEINFO)
	{
		UE_LOG(BIILogManager, Log, TEXT("[%s - %d Line] [%s()] >>> %s :: %.8d's TRACE - \n%s"), *FString(File), Line, *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
	else
	{
		UE_LOG(BIILogManager, Log, TEXT("[%s()] >>> %s :: %.8d's TRACE - \n%s"), *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
}

void Logger::Debug(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...)
{
	if (GB2LOGMANAGER_LOG_LEVEL >= B2ClientLogLevel::B2_LOG_LEVEL_DEBUG) return;
	if (LOGFILTERMGR->IsInFilter(m_namelower)) return;

	const int MAX_SIZE = 4096;
	char buffer[MAX_SIZE];

	va_list args;
	va_start(args, format);
	VSPRINTF(buffer, MAX_SIZE - 1, format, args);
	va_end(args);

	if (GB2LOGMANAGER_LOG_SHOW_FILEINFO)
	{
		UE_LOG(BIILogManager, Display, TEXT("[%s - %d Line] [%s()] >>> %s :: %.8d's DEBUG - \n%s"), *FString(File), *FString(func), Line, *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
	else
	{
		UE_LOG(BIILogManager, Display, TEXT("[%s()] >>> %s :: %.8d's DEBUG - \n%s"), *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
}
/*
void Logger::Info(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...)
{
	if (GB2LOGMANAGER_LOG_LEVEL >= B2ClientLogLevel::B2_LOG_LEVEL_INFO) return;
	if (LOGFILTERMGR->IsInFilter(m_name)) return;

	const int MAX_SIZE = 4096;
	char buffer[MAX_SIZE];

	va_list args;
	va_start(args, format);
	VSPRINTF(buffer, MAX_SIZE - 1, format, args);
	va_end(args);

	UE_LOG(BIILogManager, Display, TEXT("[%s - %d Line] %s :: %.8l INFO - %s"), *FString(func), *FString(File), Line, *FString(m_name.c_str()), LogElapsed, *FString(buffer));
}
*/

void Logger::Warning(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...)
{
	if (GB2LOGMANAGER_LOG_LEVEL >= B2ClientLogLevel::B2_LOG_LEVEL_WARN) return;
	if (LOGFILTERMGR->IsInFilter(m_namelower)) return;

	const int MAX_SIZE = 4096;
	char buffer[MAX_SIZE];

	va_list args;
	va_start(args, format);
	VSPRINTF(buffer, MAX_SIZE - 1, format, args);
	va_end(args);

	if (GB2LOGMANAGER_LOG_SHOW_FILEINFO)
	{
		UE_LOG(BIILogManager, Warning, TEXT("[%s - %d Line] [%s()] >>> %s :: %.8d's WARNING - \n%s"), *FString(File), Line, *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
	else
	{
		UE_LOG(BIILogManager, Warning, TEXT("[%s()] >>> %s :: %.8d's WARNING - \n%s"), *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
}

void Logger::Error(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...)
{
	if (GB2LOGMANAGER_LOG_LEVEL >= B2ClientLogLevel::B2_LOG_LEVEL_ERROR) return;
	if (LOGFILTERMGR->IsInFilter(m_namelower)) return;

	const int MAX_SIZE = 4096;
	char buffer[MAX_SIZE];

	va_list args;
	va_start(args, format);
	VSPRINTF(buffer, MAX_SIZE - 1, format, args);
	va_end(args);

	if (GB2LOGMANAGER_LOG_SHOW_FILEINFO)
	{
		UE_LOG(BIILogManager, Error, TEXT("[%s - %d Line] [%s()] >>> %s :: %.8d's ERROR - \n%s"), *FString(File), Line, *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
	else
	{
		UE_LOG(BIILogManager, Error, TEXT("[%s()] >>> %s :: %.8d's ERROR - \n%s"), *FString(func), *FString(m_name.c_str()), LogElapsed, *FString(buffer));
	}
}
void Logger::Blank(const ANSICHAR* File, int32 Line, const ANSICHAR* func)
{
	if (GB2LOGMANAGER_LOG_LEVEL >= B2ClientLogLevel::B2_LOG_LEVEL_ERROR) return;
	if (LOGFILTERMGR->IsInFilter(m_namelower)) return;
	
	if (GB2LOGMANAGER_LOG_SHOW_FILEINFO)
	{
		UE_LOG(BIILogManager, Display, TEXT("==========[%s - %d Line] [%s()]=========="), *FString(File), Line, *FString(func));
	}
	else
	{
		UE_LOG(BIILogManager, Display, TEXT("===============[%s()]==============="), *FString(func), *FString(m_name.c_str()));
	}
}

B2LogFilterManager::B2LogFilterManager()
{
	RemoveAllFilter();
}

void B2LogFilterManager::AddFilter(const std::string& _filterName)
{
	if (!_filterName.compare("")) return;
	std::string _filterNameLower;
	std::transform(_filterName.cbegin(), _filterName.cend(), _filterNameLower.begin(), ::tolower); // 家巩磊肺 傈券
	//Value Check
	auto result = std::find(m_filter.cbegin(), m_filter.cend(), _filterNameLower);
	if (result == m_filter.end())
		m_filter.push_back(_filterNameLower);
}

void B2LogFilterManager::RemoveFilter(const std::string& _filterName)
{
	if (!_filterName.compare("")) return;
	std::string _filterNameLower;
	std::transform(_filterName.cbegin(), _filterName.cend(), _filterNameLower.begin(), ::tolower); // 家巩磊肺 傈券
	//Value Check
	auto result = std::find(m_filter.cbegin(), m_filter.cend(), _filterNameLower);
	if (result != m_filter.end())
		m_filter.remove(_filterNameLower);
}

void B2LogFilterManager::RemoveAllFilter()
{
	m_filter.clear();
}

bool B2LogFilterManager::IsInFilter(const std::string& _filterName)
{
	if (m_filter.empty()) return false;
	if (!_filterName.compare("")) return true;

	auto result = std::find(m_filter.cbegin(), m_filter.cend(), _filterName);
	if (result != m_filter.end()) return false;

	return true;
}

void B2LogFilterManager::ShowFilterList()
{
	if (m_filter.empty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("B2LogManager >>>> Log Filter is Empty")));
		return;
	}

	StringList::const_iterator cit = m_filter.cbegin();
	while (cit != m_filter.cend())
	{
		if (!cit->compare("")) continue;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("B2LogManager >>>> Log Filter Name : [%s]"), *FString(cit->c_str())));
		cit++;
	}
}

std::shared_ptr<B2LogFilterManager> B2LogFilterManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = std::unique_ptr<B2LogFilterManager>(new B2LogFilterManager);
	}
	BII_CHECK(m_pInstance);//弊凡 老篮 芭狼 绝摆瘤父 趣矫 鞘磐 锭巩俊 磷阑荐档 乐绢辑 眉农窃
	return m_pInstance;
}

B2LogManager::B2LogManager()
{
	RemoveAllFilter();
}

B2LogManager::~B2LogManager()
{
	RemoveAllFilter();
}

void B2LogManager::AddFilter(const std::string& _filterName)
{
	LOGFILTERMGR->AddFilter(_filterName);
}

void B2LogManager::RemoveFilter(const std::string& _filterName)
{
	LOGFILTERMGR->RemoveFilter(_filterName);
}

void B2LogManager::RemoveAllFilter()
{
	LOGFILTERMGR->RemoveAllFilter();
}

void B2LogManager::ShowFilterList()
{
	LOGFILTERMGR->ShowFilterList();
}

void B2LogManager::ShowLogLevel()
{
	FString stLogLevel;
	switch (GB2LOGMANAGER_LOG_LEVEL)
	{
		case B2ClientLogLevel::B2_LOG_LEVEL_SHOWALL:
			stLogLevel = TEXT("B2_LOG_LEVEL_SHOWALL");
			break;
		case B2ClientLogLevel::B2_LOG_LEVEL_TRACE:
			stLogLevel = TEXT("B2_LOG_LEVEL_TRACE");
			break;
		case B2ClientLogLevel::B2_LOG_LEVEL_DEBUG:
			stLogLevel = TEXT("B2_LOG_LEVEL_DEBUG");
			break;
		case B2ClientLogLevel::B2_LOG_LEVEL_WARN:
			stLogLevel = TEXT("B2_LOG_LEVEL_WARN");
			break;
		case B2ClientLogLevel::B2_LOG_LEVEL_ERROR:
			stLogLevel = TEXT("B2_LOG_LEVEL_ERROR");
			break;
		case B2ClientLogLevel::B2_LOG_LEVEL_NOSHOW:
			stLogLevel = TEXT("B2_LOG_LEVEL_NOSHOW");
			break;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("B2LogManager >>>> Log Level : [%s]"), *stLogLevel));
}

Logger& B2LogManager::GetLogger(const std::string name)
{
	static std::map<std::string, LoggerPtr> loggerMap;

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

#endif //DEBUG_LOG