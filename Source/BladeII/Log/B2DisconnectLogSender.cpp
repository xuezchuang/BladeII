// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2DisconnectLogSender.h"
#include "BladeIIUtil.h"
#include "BladeIIGameMode.h"
//#include "B2GameInstance.h"

//#include "B2CodeTable.h"
//#include "Message.h"
#include "Json.h"
//#include "B2UIManager.h"

#if UE_EDITOR
#include "Logging/LogMacros.h"
DEFINE_LOG_CATEGORY_STATIC(DisconnectLog, Verbose, All);
#endif

template <class CharType>
struct TSpaceJsonPrintPolicy : public TJsonPrintPolicy<CharType>
{
	static inline void WriteLineTerminator(FArchive* Stream)
	{}

	static inline void WriteTabs(FArchive* Stream, int32 Count)
	{}

	static inline void WriteSpace(FArchive* Stream)
	{
		TJsonPrintPolicy<CharType>::WriteChar(Stream, CharType(' '));
	}
};



UB2DisconnectLogSender* UB2DisconnectLogSender::GetInstance()
{
	return Cast<UB2DisconnectLogSender>(UB2DisconnectLogSender::StaticClass()->GetDefaultObject());
}

UB2DisconnectLogSender::UB2DisconnectLogSender(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bServerSendLog = true;
}

void UB2DisconnectLogSender::DiscconectedLog(EDisconnectType Type, int32 ErrorID, FString LogStr)
{
	if (!CheckSendLog())
		return;

	//ÇÑ°³¸¸ ³²±è..
	if (LocalSaveLog.IsExgistLog())
		return;

	DisconnectLogContainer LogContainer = CreateLog(Type, ErrorID, LogStr);

	if (Type == EDisconnectType::Session)
	{
		LocalSaveLog.Log(LogContainer.SerializeLog());
		return;
	}

	SendDisconnectedLog(LogContainer);
}

bool UB2DisconnectLogSender::ExistFileLog()
{
	return LocalSaveLog.IsExgistLog();
}

void UB2DisconnectLogSender::SendRemainDisconnectedLog()
{
	if (!bClientCheckSendLog)
		return;

	TArray<FString> Logs;
	LocalSaveLog.ReadFileLog(Logs, false);

	//////////////////////////////////////////
	if (Logs.Num() > 0)
	{
		DisconnectLogContainer Container;
		if (Container.DeserializeLog(Logs[0]))
			SendDisconnectedLog(Container);
	}
}

DisconnectLogContainer UB2DisconnectLogSender::CreateLog(EDisconnectType Type, int32 ErrorID, FString LogStr)
{
	DisconnectLogContainer LogContainer;

	LogContainer.Category = TEXT("Disconnect");
	LogContainer.Action = Type == EDisconnectType::Channel ? TEXT("Channel") : TEXT("Session");
	FString AdditionalStr;
	if (LogStr.Len() != 0)
		AdditionalStr += TEXT(" - ") + LogStr;
	LogContainer.Label = TEXT("ErrorID : ") + FString::FromInt(ErrorID) + AdditionalStr;
	LogContainer.Attribute = CreateStatus();

	return LogContainer;
}

FString UB2DisconnectLogSender::CreateStatus()
{
	//////////////////////////////////////////////////////////////////////////////////
	////For json header
	//UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//ABladeIIGameMode* gameMode = nullptr;

	//EB2GameMode CurrentGameModeType(EB2GameMode::Unknown);
	//FString CurrentGameStatus;
	//if (UIMgrInst)
	//{
	//	gameMode = GetBladeIIGameMode(UIMgrInst);

	//	if (gameMode != nullptr)
	//	{
	//		CurrentGameModeType = gameMode->GetB2GameModeType();
	//		CurrentGameStatus = gameMode->GetMatchState().ToString();
	//	}
	//}

	//const FString GameModeType = B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType);
	//const FString ClientVersion(TEXT(PREPROCESSOR_TO_STRING(VERSION)));
	//const FString CLText(TEXT(PREPROCESSOR_TO_STRING(CL)));
	//const FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();

	//Add Json
	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetStringField(FString(TEXT("Client_Version")), ClientVersion);
	//JsonObject->SetStringField(FString(TEXT("CLText")), CLText);
	//JsonObject->SetStringField(FString(TEXT("GameMode")), GameModeType);
	//JsonObject->SetStringField(FString(TEXT("GameMatchState")), CurrentGameStatus);
	//JsonObject->SetStringField(FString(TEXT("Date_Time")), DateNow.ToString());
	//JsonObject->SetStringField(FString(TEXT("WIFI")), FPlatformMisc::HasActiveWiFiConnection() ? TEXT("TRUE") : TEXT("FALSE"));

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TSpaceJsonPrintPolicy<TCHAR> > > Writer = TJsonWriterFactory<TCHAR, TSpaceJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

void UB2DisconnectLogSender::SendDisconnectedLog(DisconnectLogContainer & LogContainer)
{
#if UE_EDITOR
	FString SaveLog = LogContainer.SerializeLog();

	//TEST 50% Send Log, Save File
	if (FMath::RandBool())
	{
		UE_LOG(DisconnectLog, Log, TEXT("Send To Server : %s"), *SaveLog);
		LocalSaveLog.Clear();
	}
	else
	{
		UE_LOG(DisconnectLog, Log, TEXT("SaveFile : %s"), *SaveLog);
		if(!LocalSaveLog.IsExgistLog())
			LocalSaveLog.Log(SaveLog);
	}
#elif B2_BUILD_LEVEL >= B2_BUILD_LEVEL_QA
	FString SaveLog = LogContainer.SerializeLog();

	auto EmptyMessage = data_trader::EmptyMessagePtr(new data_trader::ClientStateReportMessage(
		[this](FB2MessageInfoPtr messageInfo)
	{
		this->LocalSaveLog.Clear();
	},
		[this, SaveLog](FB2MessageInfoPtr messageInfo)
	{
		b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
		if (!this->LocalSaveLog.IsExgistLog())
			this->LocalSaveLog.Log(SaveLog);
	})
	);

	auto Message = data_trader::FillOutMessageInfo<b2network::FB2MessageInfoRequestClientStateReport>(
		EmptyMessage, LogContainer.Category, LogContainer.Action, LogContainer.Label, LogContainer.Attribute);

	Message->SendMessage();
#endif
}

void UB2DisconnectLogSender::Init(FString AccountID)
{
//#if PLATFORM_ANDROID || PLATFORM_IOS
//	DirectoryPath = FPaths::GamePersistentDownloadDir() / FString(TEXT("DisconnectedLog"));
//#else
//	DirectoryPath = FPaths::GameSavedDir() / FString(TEXT("DisconnectedLog"));
//#endif
//
//	FinalLogFileName = AccountID + TEXT("_") + SaveLogFileName;
//
//	FinalLogFileName = DirectoryPath / FinalLogFileName;
//
//	LocalSaveLog.Init(*FinalLogFileName);

	//To Do. server check
}

FString DisconnectLogContainer::SerializeLog()
{
	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField(FString(TEXT("Category")), Category);
	JsonObject->SetStringField(FString(TEXT("Action")), Action);
	JsonObject->SetStringField(FString(TEXT("Label")), Label);
	JsonObject->SetStringField(FString(TEXT("Attribute")), Attribute);
	
	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TSpaceJsonPrintPolicy<TCHAR> > > Writer = TJsonWriterFactory<TCHAR, TSpaceJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

bool DisconnectLogContainer::DeserializeLog(FString & ParseStr)
{
	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(ParseStr);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
		return false;

	Category = JsonObject->GetStringField(FString(TEXT("Category")));
	Action = JsonObject->GetStringField(FString(TEXT("Action")));
	Label = JsonObject->GetStringField(FString(TEXT("Label")));
	Attribute = JsonObject->GetStringField(FString(TEXT("Attribute")));

	return true;
}
