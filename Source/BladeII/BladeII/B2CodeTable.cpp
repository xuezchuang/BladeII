// Fill out your copyright notice in the Description page of Project Settings.


#include "B2CodeTable.h"
//#include "BladeII.h"
#include "B2PlatformAPI.h"
#include "Retailer.h"
#include "BladeIIGameMode.h"
#include "B2UIManager.h"
#include "BladeIIUtil.h"
#include "B2Version.h"
#include "Json.h"
#include "JsonObjectConverter.h"
#include "B2GameInstance.h"

B2CodeTable::B2CodeTable()
{
}

B2CodeTable::~B2CodeTable()
{
}




void B2ActionLogSender::LogSender(const int32 _LogAddresseeType)
{
	ensure(m_stCategory.Len() > 0);
	ensure(m_stAction.Len() > 0);
	ensure(_LogAddresseeType > eLogSendToFlag::LOG_SENDTO_NONE && _LogAddresseeType < eLogSendToFlag::LOG_SENDTO_TOTAL);

	if (_LogAddresseeType & eLogSendToFlag::LOG_SENDTO_GAMESERVER)
	{
		FString stActionAttr1(MakePreHeader());
		m_stActionAttr1 = stActionAttr1 + m_stActionAttr1;
		UE_LOG(LogBladeII, Log, TEXT("m_stCategory : %s\n m_stAction : %s\n m_stLabel : %s\n m_stActionAttr1 : %s"), *m_stCategory, *m_stAction, *m_stLabel, *m_stActionAttr1);
		//Direct
		//data_trader::Retailer::GetInstance().RequestClientStateReport(m_stCategory, m_stAction, m_stLabel, m_stActionAttr1);
		//or File
	}
	if(_LogAddresseeType & eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS)
	{
		B2P_SendKakaoActionLog(m_stCategory, m_stAction, m_stLabel, m_stActionAttr1);
	}
}


FString B2ActionLogSender::MakePreHeader()
{
	//////////////////////////////////////////////////////////////////////////////////
	//For json header
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	ABladeIIGameMode* gameMode = nullptr;

	EB2GameMode CurrentGameModeType(EB2GameMode::Unknown);
	if (UIMgrInst)
	{
		gameMode = GetBladeIIGameMode(UIMgrInst);

		if (gameMode != nullptr)
		{
			CurrentGameModeType = gameMode->GetB2GameModeType();
		}
	}
	//*
	FString ret;

	const FString GameModeType = B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType);
	const FString ClientVersion(PREPROCESSOR_TO_STRING(VERSION));

	ret = FString::Printf(TEXT("\"Client_Version\" : \"%s\", \"GameMode\" : \"%s\", \"Date_Time\" : \"%s\", "), *ClientVersion, *GameModeType, *(UB2GameInstance::GetKoreaTimeNow().ToString()));

	return ret;
	
	//Make Json format
	/*/
	//"SCENARIO", "COUNTERDUNGEON", "HERO_TOWER", "PVP_TAG", "PVP_TEAM", "RAID", "ASSAULT", "TUTORIAL", "GUILD", "GameModeType_NONE"
	const FString GameModeType = B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType);
	const FString ClientVersion(TEXT(PREPROCESSOR_TO_STRING(VERSION)));
	const FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();
	
	//Add Json
	TSharedPtr< FJsonObject > JsonSubObject = MakeShareable(new FJsonObject);
	JsonSubObject->SetStringField(FString(TEXT("Client_Version")), ClientVersion);
	JsonSubObject->SetStringField(FString(TEXT("GameMode")), GameModeType);
	JsonSubObject->SetStringField(FString(TEXT("Date_Time")), DateNow.ToString());

	return JsonSubObject.Get();
	//*/
	//////////////////////////////////////////////////////////////////////////////////
}

/*
FString B2ActionLogSender::ConvertJsonToFString(TSharedPtr<class FJsonObject> _jsonObj)
{
	FText jsonText;
	FJsonObjectConverter::GetTextFromObject(_jsonObj, jsonText);
	return jsonText.ToString();
}
*/