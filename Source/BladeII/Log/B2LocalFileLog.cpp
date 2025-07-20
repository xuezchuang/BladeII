// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LocalFileLog.h"
//#include "BladeII.h"

#include "Misc/FileHelper.h"

B2LocalFileLog::B2LocalFileLog()
{
}

B2LocalFileLog::~B2LocalFileLog()
{
	Flush();
}

void B2LocalFileLog::Init(const TCHAR * InFilename)
{
	if (FCString::Strlen(Filename) != 0)
		Flush();

	if (FCString::Strcmp(InFilename, Filename) == 0)
		return;

	if (InFilename)
	{
		//FCString::Strncpy(Filename, InFilename, ARRAY_COUNT(Filename));
	}
	else
	{
		Filename[0] = 0;
	}
}

bool B2LocalFileLog::Flush()
{
	if (LogCache.Num() == 0)
		return true;
	
	FString Result;
	{
		FScopeLock lock(&LogCacheCS);
		for (FString& Line : LogCache)
		{
			Result.Append(Line);
			Result.Append(LINE_TERMINATOR);
		}

		LogCache.Empty();
	}

	FScopeLock lock2(&FileReadWriteCS);
	bool result = FFileHelper::SaveStringToFile(Result, Filename, FFileHelper::EEncodingOptions::AutoDetect, 
									&IFileManager::Get(), FILEWRITE_NoReplaceExisting | FILEWRITE_Append);

	return result;

}

void B2LocalFileLog::ReadFileLog(TArray<FString>& StringResult, bool bClear)
{
	{
		FScopeLock lock(&LogCacheCS);
		StringResult.Append(LogCache);
		if(bClear)
			LogCache.Empty();
	}

	FString Result;
	{
		FScopeLock lock2(&FileReadWriteCS);
		FFileHelper::LoadFileToString(Result, Filename);

		if (bClear)
			IFileManager::Get().Delete(Filename, false, true, true);
	}

	Result.ParseIntoArrayLines(StringResult);
}

void B2LocalFileLog::Log(const TCHAR * S)
{
	Log(FString(S));
}

void B2LocalFileLog::Log(const FString & S)
{
	FScopeLock lock(&LogCacheCS);
	LogCache.Emplace(S);
}

void B2LocalFileLog::Log(const FText & S)
{
	Log(S.ToString());
}

void B2LocalFileLog::Clear()
{
	{
		FScopeLock lock(&LogCacheCS);
		LogCache.Empty();
	}

	{
		FScopeLock lock2(&FileReadWriteCS);
		IFileManager::Get().Delete(Filename, false, true, true);
	}
}

bool B2LocalFileLog::IsExgistLog()
{
	if (LogCache.Num() > 0)
		return true;
	if (IFileManager::Get().FileExists(Filename))
		return true;

	return false;
}
