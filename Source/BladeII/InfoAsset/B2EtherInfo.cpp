// Fill out your copyright notice in the Description page of Project Settings.


#include "B2EtherInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIILocText.h"

UB2EtherInfo::UB2EtherInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), TheData(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
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
	// DataTable 扁馆捞扼 Redirector 部捞瘤 臼栏妨搁 俊叼磐俊辑 TAsset 狼 碍力 肺爹捞 鞘荐利捞促.
	// 弊府 农瘤 臼绊 窍聪.. 被捞 某教 救窍绊. 弊犯霸鳖瘤 某教 救 秦档 茄锅 肺爹父 窍搁 俊叼磐俊辑 redirector 巩力 救 稗绰 巴 鞍带单..

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