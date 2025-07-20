// Fill out your copyright notice in the Description page of Project Settings.


#include "B2RewardInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2RewardInfo::UB2RewardInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
		FString RewardInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2RewardInfo"), TEXT("RewardInfoTable"), RewardInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *RewardInfoDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}

	if (TheData)
	{
		TheData->GetAllRows<FSingleRewardInfoData>(TEXT("FSingleRewardInfoData"), RewardInfoData);
	}

}

FSingleRewardInfoData* UB2RewardInfo::GetInfoData(int32 RewardIndex)
{
	if (TheData)
	{
		// RewardIndex 弊措肺 key 肺 荤侩
		FSingleRewardInfoData* FoundDataRow = TheData->FindRow<FSingleRewardInfoData>(FName(*FString::FromInt(RewardIndex)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

int32 UB2RewardInfo::GetItemRewardId(int32 ItemtemplateId)
{
	for (auto* RewardData : RewardInfoData)
	{
		if (RewardData->ItemInfoIndex == ItemtemplateId)
		{
			return RewardData->RewardId;
		}
	}
	
	return -1;
}

FString UB2RewardInfo::GetRewardNameKey(int32 RewardIndex)
{
	FSingleRewardInfoData* FoundDataRow = GetInfoData(RewardIndex);

	if (FoundDataRow)
	{
		return FoundDataRow->RewardNameForTextTable;
	}

	return TEXT("Not Found");
}

FText UB2RewardInfo::GetNameText(int32 RewardIndex)
{
	FSingleRewardInfoData* FoundDataRow = GetInfoData(RewardIndex);

	if (FoundDataRow)
	{
		//return BladeIIGetLOCText(FoundDataRow->TextCategory, FoundDataRow->RewardNameForTextTable);
	}

	return FText::FromString(TEXT("Not Found"));
}

int32 UB2RewardInfo::GetItemInfoIndex(int32 RewardIndex)
{
	FSingleRewardInfoData* FoundDataRow = GetInfoData(RewardIndex);

	if (FoundDataRow)
	{
		return FoundDataRow->ItemInfoIndex;
	}

	return -1;
}

UMaterialInterface* UB2RewardInfo::GetRewardIconMaterial(int32 RewardIndex)
{
	FSingleRewardInfoData* FoundDataRow = GetInfoData(RewardIndex);

	if (FoundDataRow)
	{
		UMaterialInterface* pMI;

		pMI = FoundDataRow->IconMaterial.LoadSynchronous();

		return pMI;
	}

	return NULL;
}

#if WITH_EDITOR
void UB2RewardInfo::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("[版绊] RewardInfo 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("RewardInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
	}

}

void UB2RewardInfo::EditorLoadAll()
{
	// DataTable 捞扼 redirector 贸府甫 犬角洒 窍扁 困秦 俊叼磐俊辑 TAsset 狼 full load 啊 鞘夸.
	if (TheData)
	{
		EditorAllLoadedTAssetHolder.Empty();

		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FSingleRewardInfoData* ThisInfo = TheData->FindRow<FSingleRewardInfoData>(RowKeys[RI], TEXT(""));

			if (ThisInfo)
			{
				FRewardInfoLoadedRefHolder SingleRefHolder;
				SingleRefHolder.LoadedIconMaterial = ThisInfo->IconMaterial.LoadSynchronous();
				if (SingleRefHolder.LoadedIconMaterial)
				{ // 弊成 持扁父 茄促. GC 救登档废.
					EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
				}
			}
		}
	}
}
#endif