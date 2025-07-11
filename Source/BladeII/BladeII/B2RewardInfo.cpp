// Fill out your copyright notice in the Description page of Project Settings.


#include "B2RewardInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2RewardInfo::UB2RewardInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
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
		// RewardIndex 그대로 key 로 사용
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
				FString::Printf(TEXT("[경고] RewardInfo 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("RewardInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
			));
#endif
	}

}

void UB2RewardInfo::EditorLoadAll()
{
	// DataTable 이라 redirector 처리를 확실히 하기 위해 에디터에서 TAsset 의 full load 가 필요.
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
				{ // 그냥 넣기만 한다. GC 안되도록.
					EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
				}
			}
		}
	}
}
#endif