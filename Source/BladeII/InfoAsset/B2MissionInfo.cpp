// Fill out your copyright notice in the Description page of Project Settings.


//#include "MissionManager.h"
#include "B2MissionInfo.h"
#include "BladeIILocText.h"



UB2MissionInfo::UB2MissionInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MissionInfoTable = nullptr;

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FString MissionInfoTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2MissionInfo"), TEXT("MissionInfoTable"), MissionInfoTablePath, GGameIni);
		MissionInfoTable = LoadObject<UDataTable>(NULL, *MissionInfoTablePath);
	}
}

FMissionInfo* UB2MissionInfo::GetMissionInfo(int32 MissionID, int32 OptionalStep)
{
	if (MissionInfoTable != nullptr)
	{
		FString RowKey = (OptionalStep > 0) ? FString::Printf(TEXT("%d_%d"), MissionID, OptionalStep) : FString::FromInt(MissionID);

		auto MissionInfo = MissionInfoTable->FindRow<FMissionInfo>(FName(*RowKey), TEXT(""));
		if (MissionInfo != nullptr)
			return MissionInfo;
		else
		{
			RowKey = (OptionalStep > 0) ? FString::Printf(TEXT("%d_%d"), MissionID, OptionalStep - 1) : FString::FromInt(MissionID);
			MissionInfo = MissionInfoTable->FindRow<FMissionInfo>(FName(*RowKey), TEXT(""));
			return MissionInfo;
		}
	}

	return nullptr;
}

#if WITH_EDITOR
void UB2MissionInfo::EditorLoadAll()
{
	// DataTable 이라 redirector 처리를 확실히 하기 위해 에디터에서 TAsset 의 full load 가 필요.
	if (MissionInfoTable)
	{
		EditorAllLoadedTAssetHolder.Empty();

		TArray<FName> RowKeys = MissionInfoTable->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FMissionInfo* ThisMissionInfo = MissionInfoTable->FindRow<FMissionInfo>(RowKeys[RI], TEXT(""));

			if (ThisMissionInfo)
			{
				FMissionInfoLoadedRefHolder SingleRefHolder;
				SingleRefHolder.LoadedMissionIcon = ThisMissionInfo->MissionIcon.LoadSynchronous();
				if (SingleRefHolder.LoadedMissionIcon)
				{ // 그냥 넣기만 한다. GC 안되도록.
					EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
				}
			}
		}
	}
}
#endif

FText GetMissionTypeText(int32 MissionID)
{
	//MissionPtr Mission = MissionManager::GetInstance().GetMission(MissionID);
	FString TypeKey(TEXT(""));
	//if (Mission)
	//{
	//	switch (Mission->MissionType)
	//	{
	//	case EMissionType::Daily: TypeKey = TEXT("Mission_Daily"); break;
	//	case EMissionType::Weekly : TypeKey = TEXT("Mission_Weekly"); break;
	//	case EMissionType::Monthly : TypeKey = TEXT("Mission_Monthly"); break;
	//	case EMissionType::Serial : TypeKey = TEXT("Mission_Normal"); break;
	//	case EMissionType::Event : TypeKey = TEXT("Mission_Event"); break;
	//	}
	//}

	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TypeKey);
}

FString GetMissionTitleKey(int32 MissionID, int32 Step)
{
	//static FString TitleFormat = TEXT("Title%d");

	//FString TitleKey = FString::Printf(*TitleFormat, MissionID);
	//if (Step) TitleKey = FString::Printf(TEXT("%s_%d"), *TitleKey, Step);

	FString TitleKey = FString::Printf(TEXT("Title%d"), MissionID);
	if (Step) TitleKey = FString::Printf(TEXT("%s_%d"), *TitleKey, Step);


	return TitleKey;
}

FString GetMissionDescKey(int32 MissionID, int32 Step)
{
	FString DescriptionKey = FString::Printf(TEXT("Desc%d"), MissionID);
	if (Step) DescriptionKey = FString::Printf(TEXT("%s_%d"), *DescriptionKey, Step);

	return DescriptionKey;
}
