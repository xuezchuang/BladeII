// Fill out your copyright notice in the Description page of Project Settings.


#include "B2RewardInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2RewardInfo::UB2RewardInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// ������ �ϳ��� �ε�
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
		// RewardIndex �״�� key �� ���
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
				FString::Printf(TEXT("[���] RewardInfo �������� �ٹ�ȣ�� ġ������ ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("RewardInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif
	}

}

void UB2RewardInfo::EditorLoadAll()
{
	// DataTable �̶� redirector ó���� Ȯ���� �ϱ� ���� �����Ϳ��� TAsset �� full load �� �ʿ�.
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
				{ // �׳� �ֱ⸸ �Ѵ�. GC �ȵǵ���.
					EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
				}
			}
		}
	}
}
#endif