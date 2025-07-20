// Fill out your copyright notice in the Description page of Project Settings.


#include "B2EtherSetOptionInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIILocText.h"


UB2EtherSetOptionInfo::UB2EtherSetOptionInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
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
				FString::Printf(TEXT("[版绊] GoodsTypeInfo 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("GoodsTypeInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
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

