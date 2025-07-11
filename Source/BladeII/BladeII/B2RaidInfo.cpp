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

		// DataTable �� ��� row �� iterate �ϴ� ����ε�.
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber < 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 �̸� �ᱹ ���⵵..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // ��������� �ʿ�� ���� �� ����. �ϴ� �̰� ����� ����.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[���] RaidInfoTable �������� �ٹ�ȣ�� ġ������ ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("RaidInfoTable ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
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