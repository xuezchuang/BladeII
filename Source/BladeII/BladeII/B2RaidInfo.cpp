// Fill out your copyright notice in the Description page of Project Settings.


#include "B2RaidInfo.h"
#include "FB2ErrorMessage.h"
#include "Misc/ConfigCacheIni.h"

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

		// DataTable 狼 葛电 row 甫 iterate 窍绰 规过牢淀.
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber < 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 捞搁 搬惫 咯扁档..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // 鉴辑措肺老 鞘夸绰 绝阑 巴 鞍促. 老窜 捞扒 版绊俊辑 力芭.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[版绊] RaidInfoTable 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("RaidInfoTable 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
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