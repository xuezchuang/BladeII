// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2SelectiveEtherLottery.h"
#include "FB2ErrorMessage.h"



UB2SelectiveEtherLottery::UB2SelectiveEtherLottery(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// ������ �ϳ��� �ε�
		FString SelectiveEtherLotteryTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2SelectiveEtherLottery"), TEXT("SelectiveEtherLotteryTable"), SelectiveEtherLotteryTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *SelectiveEtherLotteryTablePath);

		TheData->GetAllRows(TEXT("SelectiveEtherLotteryTable"), SelectiveEtherLotteryDatas);
#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}


#if WITH_EDITOR
void UB2SelectiveEtherLottery::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("error SelectiveEtherLotteryInfo Line Number"))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("SelectiveEtherLotteryInfo Data error"))
		));
#endif
	}
}
#endif

int32 UB2SelectiveEtherLottery::GetSelectiveEtherLotteryGrade(int32 InRefID)
{
	int32 Result = 0.0f;
	FSelectiveEtherLotteryData* SelectiveEtherLotteryData = GetInfoData(InRefID);
	if (SelectiveEtherLotteryData != NULL)
	{
		Result = SelectiveEtherLotteryData->Grade;
	}
	return Result;
}

int32 UB2SelectiveEtherLottery::GetSelectiveEtherLotteryTier(int32 InRefID)
{
	int32 Result = 0.0f;
	FSelectiveEtherLotteryData* SelectiveEtherLotteryData = GetInfoData(InRefID);
	if (SelectiveEtherLotteryData != NULL)
	{
		Result = SelectiveEtherLotteryData->Tier;
	}
	return Result;
}

FSelectiveEtherLotteryData* UB2SelectiveEtherLottery::GetInfoData(int32 InRefID)
{
	FSelectiveEtherLotteryData* Result = NULL;
	for (FSelectiveEtherLotteryData* Elem : SelectiveEtherLotteryDatas)
	{
		if (Elem->RefID == InRefID)
		{
			Result = Elem;
			break;
		}
	}
	return Result;
}

