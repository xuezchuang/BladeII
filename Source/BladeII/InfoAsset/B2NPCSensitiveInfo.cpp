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
	{ // 기본적으로 찾는 데에 InClassEnum 이 사용되지는 않는다.. 검사용임.
		if (InClassInfo->ClassVariation == ENPCClassVariation::ENCV_GuildBattle)
			return nullptr;
		FSingleNPCSensitiveData* FoundDataRow = TheData->FindRow<FSingleNPCSensitiveData>(InClassInfo->SensitiveDataRowKey, TEXT(""));
		if (!FoundDataRow)
		{
			FString WarnMessage = FString::Printf(TEXT("Failed to find NPCSensitiveInfo for NPCClassEnum %d:%d, key : %s"),
				(int32)InClassEnum, (int32)InClassInfo->ClassVariation, *(InClassInfo->SensitiveDataRowKey.ToString()));
			UE_LOG(LogBladeII, Warning, TEXT("%s"), *WarnMessage);
#if WITH_EDITOR  && !PLATFORM_MAC// 이제 NPCSensitiveInfo 를 열었으니 추가 엔트리 놓치지 않게 경고 메시지 띄우자. 
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage + TEXT("\n컴퓨터가 곧 폭발한다.")));
#endif
			return NULL;
		}
#if WITH_EDITOR && !PLATFORM_MAC
		if (FoundDataRow->MatchingNPCClass == ENPCClass::ENC_End || FoundDataRow->MatchingNPCClass != InClassEnum ||
			FoundDataRow->MatchingNPCClassVari != InClassInfo->ClassVariation)
		{
			FString WarnMessage = TEXT("NPCSensitiveInfo 데이터의 NPCClassInfo 와의 맵핑에 오류가 있다.\n\n");

			WarnMessage += FString::Printf(TEXT("Row 키 : %s\n"), *(InClassInfo->SensitiveDataRowKey.ToString()));
			WarnMessage += FString::Printf(TEXT("NPCClassEnum : ClassInfo %d, SensitiveInfo %d\n"), (int32)InClassEnum, (int32)FoundDataRow->MatchingNPCClass);
			WarnMessage += FString::Printf(TEXT("NPCClassVariaion : ClassInfo %d, SensitiveInfo %d\n"), (int32)InClassInfo->ClassVariation, (int32)FoundDataRow->MatchingNPCClassVari);

			WarnMessage += TEXT("\n컴퓨터가 곧 폭발한다."); // 이게 중요하다.

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
//			FString::Printf(TEXT("ENPCClass 혹은 ENPCClassVariation Enum 패키지 없음. 컴퓨터가 곧 폭발한다."))
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
//			FString::Printf(TEXT("NPCSensitiveInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
//			));
//#endif
//		return;
//	}
//	
//	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox();
//	if (this == StaticFindNPCSensitiveInfoTable() && MobInfoBox)
//	{
//		// Mob Info 용이므로 MobInfoBox 랑 검사. 키값과 매칭되는 항목이 있는지.
//
//		TArray<FString> WrongRowName; // 뭔가 잘못된 줄번호 모음
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
//			FString AllMessage = TEXT("NPCSensitiveInfo 에서 MobClassInfo 에 존재하지 않는 줄이름 감지:\n\n");
//
//			for (FString& ThisName : WrongRowName)
//			{
//				AllMessage += ThisName + TEXT(", ");
//			}
//			AllMessage += TEXT("\n\n컴퓨터가 곧 폭발한다."); // 이게 중요하다.
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
	//	// NPCClassInfo 가 로딩되지 않은 상황이라면 오래 걸릴 거.
	//	CheckInfoDataIntegrity();
	//}
}

bool UB2NPCSensitiveInfo::FirstTimeConstruction_EditorAddInfoData(ENPCClass InClassEnum, ENPCClassVariation InVariationEnum, FSingleNPCSensitiveData* InNewData, FName& OutSuggestedName)
{ 
	// NPCSensitiveInfo 도입 이전까지 NPCClassInfo 에 넣어 두었던 데이터들을 NPCSensitiveInfo 로 자동 이관하기 위한 임시 기능
	
	//FName SuggestedName = SuggestSensitiveInfoDataRowKey(InClassEnum, InVariationEnum);
	//if (TheData && InNewData && InClassEnum != ENPCClass::ENC_End && SuggestedName != NAME_None)
	//{
	//	OutSuggestedName = SuggestedName;
	//	FSingleNPCSensitiveData* FoundDataRow = TheData->FindRow<FSingleNPCSensitiveData>(SuggestedName, TEXT(""));
	//	if (FoundDataRow && FoundDataRow->MatchingNPCClass == InClassEnum && FoundDataRow->MatchingNPCClassVari == InVariationEnum)
	//	{
	//		//*FoundDataRow = *InNewData; 초기 구축은 되었고 NPCClassInfo 에서 전달할 데이터는 없으므로 여긴 필요없고 새로운 항목 추가하는 용도로는 남겨둠.
	//	}
	//	else
	//	{
	//		InNewData->MatchingNPCClass = InClassEnum; // 맞춰서 들어오긴 했겠지만
	//		InNewData->MatchingNPCClassVari = InVariationEnum;

	//		TheData->RowMap.Add(SuggestedName, reinterpret_cast<uint8*>(InNewData)); // Hope this works..
	//	}
	//	TheData->MarkPackageDirty();
	//	return true;
	//}
	return false;
}

#endif