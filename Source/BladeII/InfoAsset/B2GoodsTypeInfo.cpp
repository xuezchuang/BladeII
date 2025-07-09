// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2GoodsTypeInfo.h"


#include "FB2ErrorMessage.h"

UB2GoodsTypeInfo::UB2GoodsTypeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
		FString GoodsTypeInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2GoodsTypeInfo"), TEXT("GoodsTypeInfoTable"), GoodsTypeInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *GoodsTypeInfoDataTablePath);

		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FSingleGoodsTypeInfoData* FoundDataRow = TheData->FindRow<FSingleGoodsTypeInfoData>(RowKeys[RI], TEXT(""));
			if (FoundDataRow)
			{
				m_TheDataMap.Add(FoundDataRow->FGoodsType, FoundDataRow);
			}
		}

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

void UB2GoodsTypeInfo::PostLoad()
{
}

FSingleGoodsTypeInfoData* UB2GoodsTypeInfo::GetInfoData(FName _GoodsType)
{
	/*
	if (TheData)
	{
		FSingleGoodsTypeInfoData* FoundDataRow = TheData->FindRow<FSingleGoodsTypeInfoData>(FName(*FString::FromInt(_GoodsType)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
	/*/
	if (m_TheDataMap.Num() > 0)
	{
		if (m_TheDataMap.Contains(_GoodsType) )
		{
			FSingleGoodsTypeInfoData* FoundDataRow = m_TheDataMap[_GoodsType];
			return FoundDataRow;
		}
		return NULL;
	}
	return NULL;
	//*/
}

#if WITH_EDITOR
void UB2GoodsTypeInfo::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("[경고] GoodsTypeInfo 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("GoodsTypeInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
		));
#endif
	}
}
#endif