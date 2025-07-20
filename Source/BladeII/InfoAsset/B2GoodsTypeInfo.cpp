// Fill out your copyright notice in the Description page of Project Settings.


#include "B2GoodsTypeInfo.h"
#include "FB2ErrorMessage.h"
#include "Misc/ConfigCacheIni.h"

UB2GoodsTypeInfo::UB2GoodsTypeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
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