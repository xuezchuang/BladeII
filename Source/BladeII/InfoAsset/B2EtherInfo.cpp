// Fill out your copyright notice in the Description page of Project Settings.


#include "B2EtherInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIILocText.h"

UB2EtherInfo::UB2EtherInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
		FString EtherInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2EtherInfo"), TEXT("EtherInfoTable"), EtherInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *EtherInfoDataTablePath);

		TheData->GetAllRows(TEXT("EtherTable"), EtherDatas);
#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FEtherInfoData* UB2EtherInfo::GetInfoData(FName InGoodsType)
{
	FEtherInfoData* Result = NULL;
	for (FEtherInfoData* Elem : EtherDatas)
	{
		if (Elem->FGoodsType.Compare(InGoodsType) == 0)
		{
			Result = Elem;
			break;
		}
	}
	return Result;
}

int32 UB2EtherInfo::GetTypeID(int32 TypeName)
{
	for (FEtherInfoData* Elem : EtherDatas)
	{
		if (Elem->TypeName.Equals(FString::FromInt(TypeName)))
		{
			return Elem->TypeID;
		}
	}
	return -1;
}

FEtherInfoData* UB2EtherInfo::GetInfoData(int32 InTypeID)
{
	FEtherInfoData* Result = NULL;
	for (FEtherInfoData* Elem : EtherDatas)
	{
		if (Elem->TypeID == InTypeID)
		{
			Result = Elem;
			break;
		}
	}
	return Result;
}

FString UB2EtherInfo::GetEtherName(int32 InTypeID)
{
	FString Result;
	FEtherInfoData* EtherData = GetInfoData(InTypeID);
	if (EtherData != NULL)
	{
		Result = BladeIIGetLOCText(B2LOC_CAT_ETHER, EtherData->EtherName).ToString();
	}
	return Result;
}

FString UB2EtherInfo::GetEtherTypeName(int32 InTypeID)
{
	FString Result;
	FEtherInfoData* EtherData = GetInfoData(InTypeID);
	if (EtherData != NULL)
	{
		Result = BladeIIGetLOCText(B2LOC_CAT_ETHER, EtherData->TypeName).ToString(); //EtherData->TypeName;
	}
	return Result;
}

UMaterialInterface* UB2EtherInfo::GetEtherIcon(int32 InTypeID)
{
	UMaterialInterface* Result = NULL;
	FEtherInfoData* EtherData = GetInfoData(InTypeID);
	if (EtherData != NULL)
	{
		//GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherPosBGMtrl[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));
		Result = EtherData->IconMaterial.LoadSynchronous();
	}
	return Result;
}

TArray<int32> UB2EtherInfo::GetTypes()
{
	TArray<int32> Result;
	for (FEtherInfoData* Elem : EtherDatas)
	{
		Result.Add(Elem->TypeID);
	}
	return Result;
}

void UB2EtherInfo::PostLoad()
{

}

#if WITH_EDITOR
void UB2EtherInfo::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("error EtherInfo Line Number"))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("EtherInfo Data error"))
		));
#endif
	}
}
void UB2EtherInfo::EditorLoadAll()
{
	// DataTable 기반이라 Redirector 꼬이지 않으려면 에디터에서 TAsset 의 강제 로딩이 필수적이다.
	// 그리 크지 않고 하니.. 굳이 캐싱 안하고. 그렇게까지 캐싱 안 해도 한번 로딩만 하면 에디터에서 redirector 문제 안 겪는 것 같던데..

	if (TheData)
	{
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FEtherInfoData* FoundData = TheData->FindRow<FEtherInfoData>(RowKeys[RI], TEXT(""));
			if (FoundData)
			{
				FoundData->IconMaterial.LoadSynchronous();
			}
		}
	}
}
#endif