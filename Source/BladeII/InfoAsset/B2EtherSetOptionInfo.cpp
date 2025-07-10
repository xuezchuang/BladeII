// Fill out your copyright notice in the Description page of Project Settings.


#include "B2EtherSetOptionInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIILocText.h"


UB2EtherSetOptionInfo::UB2EtherSetOptionInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
		FString EtherSetOptionInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2EtherSetOptionInfo"), TEXT("EtherSetOptionInfoTable"), EtherSetOptionInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *EtherSetOptionInfoDataTablePath);

		TheData->GetAllRows(TEXT("EtherSetOptionInfoTable"), EtherSetOptionDatas);
#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FText UB2EtherSetOptionInfo::GetEtherSetOptionName(int32 InSetOptionID)
{
	FText Result;
	FEtherSetOptionInfoData* EtherData = GetInfoData(InSetOptionID);
	if (EtherData != NULL)
	{
		Result = BladeIIGetLOCText(B2LOC_CAT_ETHER, EtherData->EtherSetOptionName);
	}
	return Result;
}

FText UB2EtherSetOptionInfo::GetEtherSetOptionDesc(int32 InSetOptionID)
{
	FText Result;
	FEtherSetOptionInfoData* EtherData = GetInfoData(InSetOptionID);
	if (EtherData != NULL)
	{
		Result = BladeIIGetLOCText(B2LOC_CAT_ETHER, EtherData->EtherSetOptionDesc);
	}
	return Result;
}

void UB2EtherSetOptionInfo::PostLoad()
{

}

#if WITH_EDITOR
void UB2EtherSetOptionInfo::CheckInfoDataIntegrity()
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

FEtherSetOptionInfoData* UB2EtherSetOptionInfo::GetInfoData(int32 InSetOptionID)
{
	FEtherSetOptionInfoData* Result = NULL;
	for (FEtherSetOptionInfoData* Elem : EtherSetOptionDatas)
	{
		if (Elem->SetOptionID == InSetOptionID)
		{
			Result = Elem;
			break;
		}
	}
	return Result;
}

