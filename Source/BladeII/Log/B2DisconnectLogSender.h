// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2LocalFileLog.h"
#include "B2DisconnectLogSender.generated.h"

enum EDisconnectType
{
	Session,
	Channel
};


struct DisconnectLogContainer
{
	FString Category;
	FString Action;
	FString Label;
	FString Attribute;

	FString SerializeLog();
	bool DeserializeLog(FString& ParseStr);
};

UCLASS(config = Game)
class UB2DisconnectLogSender : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static UB2DisconnectLogSender* GetInstance();

	void Init(FString AccountID);

	void DiscconectedLog(EDisconnectType Type, int32 ErrorID, FString LogStr);

	bool ExistFileLog();

	void SendRemainDisconnectedLog();

	FORCEINLINE bool CheckSendLog() { return bClientCheckSendLog && bServerSendLog; }

private:
	DisconnectLogContainer CreateLog(EDisconnectType Type, int32 ErrorID, FString LogStr);
	FString CreateStatus();
	void SendDisconnectedLog(DisconnectLogContainer& LogContainer);

private:
	UPROPERTY(config)
		FString SaveLogFileName;

	UPROPERTY(config)
		bool bClientCheckSendLog;

	bool bServerSendLog;

	FString FinalLogFileName;
	FString DirectoryPath;

private:
	B2LocalFileLog LocalSaveLog;
};