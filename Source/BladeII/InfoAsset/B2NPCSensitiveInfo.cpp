// Fill out your copyright notice in the Description page of Project Settings.


#include "B2NPCSensitiveInfo.h"
#include "B2NPCClassInfo.h"
#include "B2NPCClassInfoBox.h"
//#if WITH_EDITOR
//#include "AssetEditorManager.h"
//#endif

#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2NPCSensitiveInfo::UB2NPCSensitiveInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2NPCSensitiveInfo::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	TheData = InDataTable;

//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		FAssetEditorManager::Get().OnAssetEditorOpened().AddUObject(this, &UB2NPCSensitiveInfo::OnAssetEditorOpen);
//	}
//#endif
}

FSingleNPCSensitiveData* UB2NPCSensitiveInfo::GetInfoData(ENPCClass InClassEnum, UB2NPCSingleClassInfo* InClassInfo)
{
	if (InClassInfo && TheData)
	{ // 扁夯利栏肺 茫绰 单俊 InClassEnum 捞 荤侩登瘤绰 臼绰促.. 八荤侩烙.
		if (InClassInfo->ClassVariation == ENPCClassVariation::ENCV_GuildBattle)
			return nullptr;
		FSingleNPCSensitiveData* FoundDataRow = TheData->FindRow<FSingleNPCSensitiveData>(InClassInfo->SensitiveDataRowKey, TEXT(""));
		if (!FoundDataRow)
		{
			FString WarnMessage = FString::Printf(TEXT("Failed to find NPCSensitiveInfo for NPCClassEnum %d:%d, key : %s"),
				(int32)InClassEnum, (int32)InClassInfo->ClassVariation, *(InClassInfo->SensitiveDataRowKey.ToString()));
			UE_LOG(LogBladeII, Warning, TEXT("%s"), *WarnMessage);
#if WITH_EDITOR  && !PLATFORM_MAC// 捞力 NPCSensitiveInfo 甫 凯菌栏聪 眠啊 浚飘府 初摹瘤 臼霸 版绊 皋矫瘤 剁快磊. 
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage + TEXT("\n哪腔磐啊 梆 气惯茄促.")));
#endif
			return NULL;
		}
#if WITH_EDITOR && !PLATFORM_MAC
		if (FoundDataRow->MatchingNPCClass == ENPCClass::ENC_End || FoundDataRow->MatchingNPCClass != InClassEnum ||
			FoundDataRow->MatchingNPCClassVari != InClassInfo->ClassVariation)
		{
			FString WarnMessage = TEXT("NPCSensitiveInfo 单捞磐狼 NPCClassInfo 客狼 甘俏俊 坷幅啊 乐促.\n\n");

			WarnMessage += FString::Printf(TEXT("Row 虐 : %s\n"), *(InClassInfo->SensitiveDataRowKey.ToString()));
			WarnMessage += FString::Printf(TEXT("NPCClassEnum : ClassInfo %d, SensitiveInfo %d\n"), (int32)InClassEnum, (int32)FoundDataRow->MatchingNPCClass);
			WarnMessage += FString::Printf(TEXT("NPCClassVariaion : ClassInfo %d, SensitiveInfo %d\n"), (int32)InClassInfo->ClassVariation, (int32)FoundDataRow->MatchingNPCClassVari);

			WarnMessage += TEXT("\n哪腔磐啊 梆 气惯茄促."); // 捞霸 吝夸窍促.

			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage));
		}
#endif
		return FoundDataRow;
	}
	return NULL;
}

FName UB2NPCSensitiveInfo::SuggestSensitiveInfoDataRowKey(ENPCClass InClassEnum, ENPCClassVariation InVariationEnum)
{
//	const UEnum* NPCClassEnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASS_ENUM_PACKAGE_NAME, true);
//	const UEnum* NPCVariEnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASSVARI_ENUM_PACKAGE_NAME, true);
//
//	if (!NPCClassEnumPtr || !NPCVariEnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("ENPCClass 趣篮 ENPCClassVariation Enum 菩虐瘤 绝澜. 哪腔磐啊 梆 气惯茄促."))
//			));
//#endif
//		return NAME_None;
//	}
//
//	FString RetNameString = FString::Printf(TEXT("%s_%s"),
//		*NPCClassEnumPtr->GetNameStringByIndex((int32)InClassEnum).RightChop(FString(ENPCCLASSENUM_COMMON_PREFIX).Len()),
//		*NPCVariEnumPtr->GetNameStringByIndex((int32)InVariationEnum).RightChop(FString(ENPCCLASSVARIENUM_COMMON_PREFIX).Len())
//		);
//
//	return FName(*RetNameString);
	return FName(TEXT(""));
}

#if WITH_EDITOR
void UB2NPCSensitiveInfo::CheckInfoDataIntegrity()
{

//	if (!TheData)
//	{
//#if !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("NPCSensitiveInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
//			));
//#endif
//		return;
//	}
//	
//	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox();
//	if (this == StaticFindNPCSensitiveInfoTable() && MobInfoBox)
//	{
//		// Mob Info 侩捞骨肺 MobInfoBox 尔 八荤. 虐蔼苞 概莫登绰 亲格捞 乐绰瘤.
//
//		TArray<FString> WrongRowName; // 购啊 肋给等 临锅龋 葛澜
//		TArray<FName> RowKeys = TheData->GetRowNames();
//		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
//		{
//			UB2NPCSingleClassInfo* FoundClassInfo = MobInfoBox->TryGetMatchingClassInfo(RowKeys[RI]);
//			if (!FoundClassInfo)
//			{
//				WrongRowName.Add(RowKeys[RI].ToString());
//			}
//		}
//
//		if (WrongRowName.Num() > 0)
//		{
//#if !PLATFORM_MAC
//			FString AllMessage = TEXT("NPCSensitiveInfo 俊辑 MobClassInfo 俊 粮犁窍瘤 臼绰 临捞抚 皑瘤:\n\n");
//
//			for (FString& ThisName : WrongRowName)
//			{
//				AllMessage += ThisName + TEXT(", ");
//			}
//			AllMessage += TEXT("\n\n哪腔磐啊 梆 气惯茄促."); // 捞霸 吝夸窍促.
//
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(AllMessage));
//#endif
//		}
//	}

}

void UB2NPCSensitiveInfo::OnAssetEditorOpen(UObject* InAsset)
{
	//UDataTable* CastedDataTable = Cast<UDataTable>(InAsset);

	//if (CastedDataTable && CastedDataTable->GetRowStructName() == FName(TEXT("SingleNPCSensitiveData")))
	//{
	//	// NPCClassInfo 啊 肺爹登瘤 臼篮 惑炔捞扼搁 坷贰 吧副 芭.
	//	CheckInfoDataIntegrity();
	//}
}

bool UB2NPCSensitiveInfo::FirstTimeConstruction_EditorAddInfoData(ENPCClass InClassEnum, ENPCClassVariation InVariationEnum, FSingleNPCSensitiveData* InNewData, FName& OutSuggestedName)
{ 
	// NPCSensitiveInfo 档涝 捞傈鳖瘤 NPCClassInfo 俊 持绢 滴菌带 单捞磐甸阑 NPCSensitiveInfo 肺 磊悼 捞包窍扁 困茄 烙矫 扁瓷
	
	//FName SuggestedName = SuggestSensitiveInfoDataRowKey(InClassEnum, InVariationEnum);
	//if (TheData && InNewData && InClassEnum != ENPCClass::ENC_End && SuggestedName != NAME_None)
	//{
	//	OutSuggestedName = SuggestedName;
	//	FSingleNPCSensitiveData* FoundDataRow = TheData->FindRow<FSingleNPCSensitiveData>(SuggestedName, TEXT(""));
	//	if (FoundDataRow && FoundDataRow->MatchingNPCClass == InClassEnum && FoundDataRow->MatchingNPCClassVari == InVariationEnum)
	//	{
	//		//*FoundDataRow = *InNewData; 檬扁 备绵篮 登菌绊 NPCClassInfo 俊辑 傈崔且 单捞磐绰 绝栏骨肺 咯变 鞘夸绝绊 货肺款 亲格 眠啊窍绰 侩档肺绰 巢败狄.
	//	}
	//	else
	//	{
	//		InNewData->MatchingNPCClass = InClassEnum; // 嘎苗辑 甸绢坷变 沁摆瘤父
	//		InNewData->MatchingNPCClassVari = InVariationEnum;

	//		TheData->RowMap.Add(SuggestedName, reinterpret_cast<uint8*>(InNewData)); // Hope this works..
	//	}
	//	TheData->MarkPackageDirty();
	//	return true;
	//}
	return false;
}

#endif