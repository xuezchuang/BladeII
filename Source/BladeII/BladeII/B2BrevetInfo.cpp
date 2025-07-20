// Fill out your copyright notice in the Description page of Project Settings.


#include "B2BrevetInfo.h"
//#include "BladeII.h"

#include "FB2ErrorMessage.h"
#include "Misc/ConfigCacheIni.h"
#include "Materials/MaterialInterface.h"

UB2BrevetInfo::UB2BrevetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
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
		// BrevetRank 弊措肺 key 肺 荤侩
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
				FString::Printf(TEXT("[版绊] BrevetInfo 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("BrevetInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
	}
}
#endif