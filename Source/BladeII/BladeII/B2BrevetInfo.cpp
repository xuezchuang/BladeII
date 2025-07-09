// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2BrevetInfo.h"


#include "FB2ErrorMessage.h"

UB2BrevetInfo::UB2BrevetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
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
		// BrevetRank 그대로 key 로 사용
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
				FString::Printf(TEXT("[경고] BrevetInfo 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("BrevetInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
			));
#endif
	}
}
#endif