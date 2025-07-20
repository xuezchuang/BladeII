// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef _B2_LOGMANAGER_H_
#define _B2_LOGMANAGER_H_
//
//#include "string"
//#include "list"


#define DEBUG_LOG 0

extern uint8 g_B2LogManager_Log_Level;
extern bool g_B2LogManager_Log_Show_FileInfo;
#define GB2LOGMANAGER_LOG_LEVEL g_B2LogManager_Log_Level
#define GB2LOGMANAGER_LOG_SHOW_FILEINFO g_B2LogManager_Log_Show_FileInfo

DECLARE_LOG_CATEGORY_EXTERN(BIILogManager, Log, All);

enum B2ClientLogLevel
{
	B2_LOG_LEVEL_SHOWALL = 0,
	B2_LOG_LEVEL_TRACE		= 1,
	B2_LOG_LEVEL_DEBUG		= 2,
//	B2_LOG_LEVEL_INFO				= 3,
	B2_LOG_LEVEL_WARN		= 3,	//4,
	B2_LOG_LEVEL_ERROR		= 4,	//5,
//	B2_LOG_LEVEL_FATAL				= 6,
	B2_LOG_LEVEL_NOSHOW,
};

#if !DEBUG_LOG
#define B2GetLogger(NAME)
#define B2LOGMANAGER_TRACE(Logger, Format, ...)
#define B2LOGMANAGER_DEBUG(Logger, Format, ...)
#define B2LOGMANAGER_INFO(Logger, Format, ...)
#define B2LOGMANAGER_WARNING(Logger, Format, ...)
#define B2LOGMANAGER_ERROR(Logger, Format, ...)
#define B2LOGMANAGER_BLANK(Logger)
#else
//盔贰 logger肺 沥狼 窍妨 窍看栏唱 蜡聪萍呼靛 漂己惑 颇老阑 钦摹绰 苞沥俊辑 犁沥狼啊 登绰 巩力啊 乐绢 第俊 捞抚阑 嘿烙
#define B2GetLogger(NAME) static auto Logger##NAME = B2Logger::B2LogManager::GetLogger(#NAME);

#define B2LOGMANAGER_TRACE(Logger, Format, ...) \
    { \
        Logger.Trace(__FILE__, __LINE__, __FUNCTION__, Format, ##__VA_ARGS__); \
    }
#define B2LOGMANAGER_DEBUG(Logger, Format, ...) \
    { \
        Logger.Debug(__FILE__, __LINE__, __FUNCTION__, Format, ##__VA_ARGS__); \
    }
//#define B2LOGMANAGER_INFO(Logger, Format, ...) \
//    { \
//        Logger.Info(__FILE__, __LINE__, __FUNCTION__, Format, ##__VA_ARGS__); \
//    }
#define B2LOGMANAGER_WARNING(Logger, Format, ...) \
    { \
        Logger.Warning(__FILE__, __LINE__, __FUNCTION__, Format, ##__VA_ARGS__); \
    }
#define B2LOGMANAGER_ERROR(Logger, Format, ...) \
    { \
        Logger.Error(__FILE__, __LINE__, __FUNCTION__, Format, ##__VA_ARGS__); \
    }
#define B2LOGMANAGER_BLANK(Logger) \
    { \
        Logger.Blank(__FILE__, __LINE__, __FUNCTION__); \
    }
#endif

/**
 * 
 */
namespace B2Logger
{
	class Logger
	{
#if DEBUG_LOG

	public:
		Logger(const std::string name);

		void Trace(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...);
		void Debug(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...);
		//void Info(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...);
		void Warning(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...);
		void Error(const ANSICHAR* File, int32 Line, const ANSICHAR* func, const char* format, ...);
		void Blank(const ANSICHAR* File, int32 Line, const ANSICHAR* func);

	protected:
	private:

	public:
	protected:
	private:
		static long m_startTick;

		const std::string m_name;
		std::string m_namelower;
#endif //DEBUG_LOG
	};

	class B2LogFilterManager
	{
#if DEBUG_LOG
		friend class Logger;
		friend class B2LogManager;
		typedef std::list<std::string> StringList;

	public:
		explicit B2LogFilterManager();
		~B2LogFilterManager() { RemoveAllFilter(); };

		void AddFilter(const std::string& _filterName);
		void RemoveFilter(const std::string& _filterName);
		void RemoveAllFilter();
		bool IsInFilter(const std::string& _filterName);
		void ShowFilterList();

	protected:
		static std::shared_ptr<B2LogFilterManager> GetInstance();
	private:

	public:
	protected:
	private:
		StringList m_filter;
		static std::shared_ptr<B2LogFilterManager> m_pInstance;
#endif //DEBUG_LOG
	};

	class B2LogManager
	{
#if DEBUG_LOG
	typedef std::shared_ptr<B2Logger::Logger> LoggerPtr;

	public:
		explicit B2LogManager();
		~B2LogManager();

		static void AddFilter(const std::string& _filterName);
		static void RemoveFilter(const std::string& _filterName);
		static void RemoveAllFilter();
		static void ShowFilterList();
		static void ShowLogLevel();

		static Logger& GetLogger(const std::string& name);
	protected:
	private:

	public:
	protected:
	private:
#endif //DEBUG_LOG
	};
}

#endif //_B2_LOGMANAGER_H_