// Fill out your copyright notice in the Description page of Project Settings.


#include "B2SeasonEventInfo.h"
#include "Internationalization/Culture.h"

#include "Internationalization/Internationalization.h"
#include "Misc/ConfigCacheIni.h"

//#include "B2UILinkManager.h"

UB2SeasonEventInfo::UB2SeasonEventInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	LoadSeasonEventInfoTable();

	FCultureRef fCurretCulture = FInternationalization::Get().GetCurrentCulture();

	strNativeCulture = fCurretCulture->GetName();
}

UDataTable* UB2SeasonEventInfo::LoadSeasonEventInfoTable()
{
	if (CachedSeasonEventInfoTable == nullptr)
	{
		FString SeasonEventInfoTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2SeasonEventInfo"), TEXT("SeasonEventInfoTable"), SeasonEventInfoTablePath, GGameIni);
		CachedSeasonEventInfoTable = LoadObject<UDataTable>(nullptr, *SeasonEventInfoTablePath);
	}

	return CachedSeasonEventInfoTable;
}

void UB2SeasonEventInfo::GotoLinkUIScene(int32 iDay, int32 iIndex)
{
	if (UDataTable* SeasonEventInfoTable = LoadSeasonEventInfoTable())
	{
		if (FSeasonEventInfo* pSeasonEventInfo = FindSeasonEventInfo(iDay, iIndex))
		{
			//FB2UILinkManager::LinkUIScene(pSeasonEventInfo->UIScene, pSeasonEventInfo->Optional1, pSeasonEventInfo->Optional2);
		}
	}
}

FText UB2SeasonEventInfo::GetSeasonEventSlotText(int32 iDay, int32 iIndex)
{
	FText fSeasonEventSlotText;

	if (UDataTable* SeasonEventInfoTable = LoadSeasonEventInfoTable())
	{
		if (FSeasonEventInfo* pSeasonEventInfo = FindSeasonEventInfo(iDay, iIndex))
		{
			if (strNativeCulture == TEXT("en"))
				fSeasonEventSlotText = pSeasonEventInfo->Text_English;
			else if (strNativeCulture == TEXT("ko"))
				fSeasonEventSlotText = pSeasonEventInfo->Text_Korean;
			else
				fSeasonEventSlotText = pSeasonEventInfo->Text_Other;
		}
	}

	return fSeasonEventSlotText;
}

FSeasonEventInfo* UB2SeasonEventInfo::FindSeasonEventInfo(int32 iDay, int32 iIndex)
{
	FSeasonEventInfo* SeasonEventInfo = nullptr;

	//if (UDataTable* SeasonEventInfoTable = LoadSeasonEventInfoTable())
	//{
	//	FString KeyFormat(TEXT("Day%d_Index%d"));

	//	SeasonEventInfo = SeasonEventInfoTable->FindRow<FSeasonEventInfo>(FName(*FString::Printf(*KeyFormat, iDay, iIndex)), TEXT(""));
	//}

	return SeasonEventInfo;
}

#if WITH_EDITOR
void UB2SeasonEventInfo::EditorLoadAll()
{

}
#endif