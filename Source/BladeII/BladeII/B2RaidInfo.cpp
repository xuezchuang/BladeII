// Fill out your copyright notice in the Description page of Project Settings.


#include "B2RaidInfo.h"


#include "FB2ErrorMessage.h"

UB2RaidInfoTable::UB2RaidInfoTable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FString RaidInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2RaidInfo"), TEXT("RaidInfoTable"), RaidInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *RaidInfoDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FRaidInfoTableData* UB2RaidInfoTable::GetInfoData(int32 Idx)
{
	if (TheData)
		return TheData->FindRow<FRaidInfoTableData>(FName(*FString::FromInt(Idx)), TEXT(""));

	return NULL;
}

#if WITH_EDITOR
void UB2RaidInfoTable::CheckInfoDataIntegrity()
{
	if (TheData)
	{
		bool bHasRowKeyLessThan0 = false;
		bool bHasNonSequentialRowKey = false;

		// DataTable 의 모든 row 를 iterate 하는 방법인듯.
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber < 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 이면 결국 여기도..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // 순서대로일 필요는 없을 것 같다. 일단 이건 경고에서 제거.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[경고] RaidInfoTable 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("RaidInfoTable 데이터 없음. 컴퓨터가 곧 폭발한다."))
			));
#endif
	}
}
#endif

FText UB2RaidInfoTable::GetRaidNameByRaidMapId(int32 InRaidMapId, int32 InRaidStep)
{
	if (auto* TableDatas = GetInfoData(GetRaidKey(InRaidMapId,InRaidStep)))
		return TableDatas->RaidName;

	return FText();
}

FText UB2RaidInfoTable::GetRaidDescByRaidMapId(int32 InRaidMapId, int32 InRaidStep)
{
	if (auto* TableDatas = GetInfoData(GetRaidKey(InRaidMapId, InRaidStep)))
		return TableDatas->RaidDesc;

	return FText();
}

int32 UB2RaidInfoTable::GetClientStageIdByRaidMapId(int32 InRaidMapId, int32 InRaidStep)
{
	if (auto* TableDatas = GetInfoData(GetRaidKey(InRaidMapId, InRaidStep)))
		return TableDatas->ClientStageId;

	return 1;
}