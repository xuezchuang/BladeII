// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Multi Thread O, Async X

class B2LocalFileLog
{
public:
	B2LocalFileLog();
	~B2LocalFileLog();

	void Init(const TCHAR* InFilename);

	bool Flush();
	void Clear();

	bool IsExgistLog();

	void ReadFileLog(TArray<FString>& StringResult, bool bClear);

	void Log(const TCHAR* S);
	void Log(const FString& S);
	void Log(const FText& S);

private:
	TCHAR Filename[1024];

	TArray<FString> LogCache;
	TSharedPtr<FArchive> FileArchive;

	FCriticalSection LogCacheCS;
	FCriticalSection FileReadWriteCS;
};