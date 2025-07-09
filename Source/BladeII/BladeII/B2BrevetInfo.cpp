// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2BrevetInfo.h"


#include "FB2ErrorMessage.h"

UB2BrevetInfo::UB2BrevetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// ������ �ϳ��� �ε�
		FString BrevetInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2BrevetInfo"), TEXT("BrevetInfoTable"), BrevetInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *BrevetInfoDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FSingleBrevetInfoData* UB2BrevetInfo::GetInfoData(int32 BrevetRank)
{
	if (TheData)
	{
		// BrevetRank �״�� key �� ���
		FSingleBrevetInfoData* FoundDataRow = TheData->FindRow<FSingleBrevetInfoData>(FName(*FString::FromInt(BrevetRank)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

FString UB2BrevetInfo::GetBrevetRankNameKey(int32 BrevetRank)
{
	FSingleBrevetInfoData* FoundDataRow = GetInfoData(BrevetRank);

	if (FoundDataRow)
	{
		return FoundDataRow->RankNameForTextTable;
	}

	return TEXT("Not Found");
}

int32 UB2BrevetInfo::GetDataCount() const
{
	return TheData ? TheData->GetRowNames().Num() - 1 : -1;
}

int32 UB2BrevetInfo::GetMaxBrevetRank() const
{
	return GetDataCount();
}

UMaterialInterface* UB2BrevetInfo::GetBrevetRankIconMaterial(int32 BrevetRank, bool isBigSize)
{
	FSingleBrevetInfoData* FoundDataRow = GetInfoData(BrevetRank);

	if (FoundDataRow)
	{
		UMaterialInterface* pMI;

		if (isBigSize)
			pMI = FoundDataRow->IconMaterialBig.LoadSynchronous();
		else
			pMI = FoundDataRow->IconMaterialSmall.LoadSynchronous();

		return pMI;
	}

	return NULL;
}

#if WITH_EDITOR
void UB2BrevetInfo::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("[���] BrevetInfo �������� �ٹ�ȣ�� ġ������ ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("BrevetInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif
	}
}
#endif