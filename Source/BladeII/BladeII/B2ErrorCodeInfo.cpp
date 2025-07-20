// Fill out your copyright notice in the Description page of Project Settings.
#include "B2ErrorCodeInfo.h"
//#include "BladeII.h"
#include "Misc/ConfigCacheIni.h"
#include "FB2ErrorMessage.h"

UB2ErrorCodeInfo::UB2ErrorCodeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
		FString ErrorCodeInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2ErrorCodeInfo"), TEXT("ErrorCodeInfoTable"), ErrorCodeInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *ErrorCodeInfoDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FSingleErrorCodeInfoData* UB2ErrorCodeInfo::GetInfoData(int32 nErrorCode)
{
	if (TheData)
	{
		FSingleErrorCodeInfoData* FoundDataRow = TheData->FindRow<FSingleErrorCodeInfoData>(FName(*FString::FromInt(nErrorCode)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

#if WITH_EDITOR
void UB2ErrorCodeInfo::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("[版绊] ErrorCodeInfo 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ErrorCodeInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
	}
}
#endif