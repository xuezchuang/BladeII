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
	{ // �⺻������ ã�� ���� InClassEnum �� �������� �ʴ´�.. �˻����.
		if (InClassInfo->ClassVariation == ENPCClassVariation::ENCV_GuildBattle)
			return nullptr;
		FSingleNPCSensitiveData* FoundDataRow = TheData->FindRow<FSingleNPCSensitiveData>(InClassInfo->SensitiveDataRowKey, TEXT(""));
		if (!FoundDataRow)
		{
			FString WarnMessage = FString::Printf(TEXT("Failed to find NPCSensitiveInfo for NPCClassEnum %d:%d, key : %s"),
				(int32)InClassEnum, (int32)InClassInfo->ClassVariation, *(InClassInfo->SensitiveDataRowKey.ToString()));
			UE_LOG(LogBladeII, Warning, TEXT("%s"), *WarnMessage);
#if WITH_EDITOR  && !PLATFORM_MAC// ���� NPCSensitiveInfo �� �������� �߰� ��Ʈ�� ��ġ�� �ʰ� ��� �޽��� �����. 
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage + TEXT("\n��ǻ�Ͱ� �� �����Ѵ�.")));
#endif
			return NULL;
		}
#if WITH_EDITOR && !PLATFORM_MAC
		if (FoundDataRow->MatchingNPCClass == ENPCClass::ENC_End || FoundDataRow->MatchingNPCClass != InClassEnum ||
			FoundDataRow->MatchingNPCClassVari != InClassInfo->ClassVariation)
		{
			FString WarnMessage = TEXT("NPCSensitiveInfo �������� NPCClassInfo ���� ���ο� ������ �ִ�.\n\n");

			WarnMessage += FString::Printf(TEXT("Row Ű : %s\n"), *(InClassInfo->SensitiveDataRowKey.ToString()));
			WarnMessage += FString::Printf(TEXT("NPCClassEnum : ClassInfo %d, SensitiveInfo %d\n"), (int32)InClassEnum, (int32)FoundDataRow->MatchingNPCClass);
			WarnMessage += FString::Printf(TEXT("NPCClassVariaion : ClassInfo %d, SensitiveInfo %d\n"), (int32)InClassInfo->ClassVariation, (int32)FoundDataRow->MatchingNPCClassVari);

			WarnMessage += TEXT("\n��ǻ�Ͱ� �� �����Ѵ�."); // �̰� �߿��ϴ�.

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
//			FString::Printf(TEXT("ENPCClass Ȥ�� ENPCClassVariation Enum ��Ű�� ����. ��ǻ�Ͱ� �� �����Ѵ�."))
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
//			FString::Printf(TEXT("NPCSensitiveInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
//			));
//#endif
//		return;
//	}
//	
//	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox();
//	if (this == StaticFindNPCSensitiveInfoTable() && MobInfoBox)
//	{
//		// Mob Info ���̹Ƿ� MobInfoBox �� �˻�. Ű���� ��Ī�Ǵ� �׸��� �ִ���.
//
//		TArray<FString> WrongRowName; // ���� �߸��� �ٹ�ȣ ����
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
//			FString AllMessage = TEXT("NPCSensitiveInfo ���� MobClassInfo �� �������� �ʴ� ���̸� ����:\n\n");
//
//			for (FString& ThisName : WrongRowName)
//			{
//				AllMessage += ThisName + TEXT(", ");
//			}
//			AllMessage += TEXT("\n\n��ǻ�Ͱ� �� �����Ѵ�."); // �̰� �߿��ϴ�.
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
	//	// NPCClassInfo �� �ε����� ���� ��Ȳ�̶�� ���� �ɸ� ��.
	//	CheckInfoDataIntegrity();
	//}
}

bool UB2NPCSensitiveInfo::FirstTimeConstruction_EditorAddInfoData(ENPCClass InClassEnum, ENPCClassVariation InVariationEnum, FSingleNPCSensitiveData* InNewData, FName& OutSuggestedName)
{ 
	// NPCSensitiveInfo ���� �������� NPCClassInfo �� �־� �ξ��� �����͵��� NPCSensitiveInfo �� �ڵ� �̰��ϱ� ���� �ӽ� ���
	
	//FName SuggestedName = SuggestSensitiveInfoDataRowKey(InClassEnum, InVariationEnum);
	//if (TheData && InNewData && InClassEnum != ENPCClass::ENC_End && SuggestedName != NAME_None)
	//{
	//	OutSuggestedName = SuggestedName;
	//	FSingleNPCSensitiveData* FoundDataRow = TheData->FindRow<FSingleNPCSensitiveData>(SuggestedName, TEXT(""));
	//	if (FoundDataRow && FoundDataRow->MatchingNPCClass == InClassEnum && FoundDataRow->MatchingNPCClassVari == InVariationEnum)
	//	{
	//		//*FoundDataRow = *InNewData; �ʱ� ������ �Ǿ��� NPCClassInfo ���� ������ �����ʹ� �����Ƿ� ���� �ʿ���� ���ο� �׸� �߰��ϴ� �뵵�δ� ���ܵ�.
	//	}
	//	else
	//	{
	//		InNewData->MatchingNPCClass = InClassEnum; // ���缭 ������ �߰�����
	//		InNewData->MatchingNPCClassVari = InVariationEnum;

	//		TheData->RowMap.Add(SuggestedName, reinterpret_cast<uint8*>(InNewData)); // Hope this works..
	//	}
	//	TheData->MarkPackageDirty();
	//	return true;
	//}
	return false;
}

#endif