#include "BladeII.h"
#include "B2ScenarioDialogInfo.h"
#include "B2PCClassInfo.h"
#include "BladeIIUtil.h"
#include "BladeIIGameMode.h"
#include "BladeIIGameImpl.h"
#include "FB2ErrorMessage.h"
#include "B2GameInstance.h"

UMaterialInstance* FDialogCharacterInfo::GetPortraitMtrlForType(FStreamableManager& InLoadManager, EPortraitType InType, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder)
{
	for (FDialogPortraitInfo& ThisPortraitInfo : CharacterPortrait)
	{
		if (ThisPortraitInfo.PortraitType == InType)
		{
			UMaterialInstance* DummyLoadedPtr = nullptr;
			GetSomeLazyLoadedAssetCommon<UMaterialInstance>(InLoadManager, ThisPortraitInfo.Portrait, &DummyLoadedPtr, TEXT("DialogCharacterInfo"));
			if (DummyLoadedPtr)
			{ // LoadedPortraitMtrls 에 넣는 것 까지는 키 검사하고 하면 넘 복잡하므로.. 이건 AddUnique 로
				InOutLoadedRefHolder.LoadedPortraitMtrls.AddUnique(DummyLoadedPtr);
			}
			return DummyLoadedPtr;
		}
	}
	return NULL;
}

UTexture2D* FDialogCharacterInfo::GetWholeBodyPortrait(FStreamableManager& InLoadManager, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder)
{ 
	return GetSomeLazyLoadedAssetCommon<UTexture2D>(InLoadManager, WholeBodyPortraitPtr, &InOutLoadedRefHolder.LoadedWholeBodyPortrait, TEXT("DialogCharacterInfo"));
}

void FDialogCharacterInfo::UnloadAll(FStreamableManager& InLoadManager, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder)
{
	for (FDialogPortraitInfo& ThisPortraitInfo : CharacterPortrait)
	{
		UMaterialInstance* DummyMtrlPtr = nullptr;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInstance>(InLoadManager, ThisPortraitInfo.Portrait, &DummyMtrlPtr); // LoadedPtrCache 는 아래서 따로 처리.
	}
	UnloadSomeLazyLoadedAssetCommon<UTexture2D>(InLoadManager, WholeBodyPortraitPtr, &InOutLoadedRefHolder.LoadedWholeBodyPortrait);
	InOutLoadedRefHolder.LoadedPortraitMtrls.Empty();
	InOutLoadedRefHolder.LoadedWholeBodyPortrait = nullptr;
}

#if WITH_EDITOR
void FDialogCharacterInfo::EditorLoadAll(FStreamableManager& InLoadManager, FDialogCharacterInfoLoadedRefHolder& InOutLoadedRefHolder)
{
	for (FDialogPortraitInfo& ThisPortraitInfo : CharacterPortrait)
	{
		// 안에서 바보같이 for 문을 또 돌게 되는데.. 심한 건 아니고 에디터 기능이니 봐줌.
		GetPortraitMtrlForType(InLoadManager, ThisPortraitInfo.PortraitType, InOutLoadedRefHolder);
	}
	GetSomeLazyLoadedAssetCommon<UTexture2D>(InLoadManager, WholeBodyPortraitPtr, &InOutLoadedRefHolder.LoadedWholeBodyPortrait, TEXT("DialogCharacterInfo"));
}
#endif

UB2DialogCharacterInfoSet::UB2DialogCharacterInfoSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2DialogCharacterInfoSet::PostLoad()
{
	Super::PostLoad();

	DialogCharacterMap.Empty(DialogCharacterInfoArray.Num());
	// 약간 특이한 구성인데 DialogCharacterInfoArray 자체가 TMap 이나 그런 식의 식별자와 같이 묶여있는 타입이 아니고
	// PostLoad 시점에 Array 접근 index 들을 미리 캐싱해 둠.
	for (int32 ArrayIndex = 0; ArrayIndex < DialogCharacterInfoArray.Num(); ++ArrayIndex)
	{
		FDialogCharacterInfo& ThisInfo = DialogCharacterInfoArray[ArrayIndex];

		if (ThisInfo.SpecifyPCClass != EPCClass::EPC_End)
			DialogPlayerClassMap.Add(ThisInfo.SpecifyPCClass, ArrayIndex);

		DialogCharacterMap.Add(ThisInfo.CharacterCodeName, ArrayIndex);
	}
}

static EPCClass LocalGetPlayerCharClass(UObject* WorldContextObject, bool bForMain /*false for Tag*/)
{
	//if (WorldContextObject)
	//{
	//	ABladeIIPlayer* LocalPlayerCharacter = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(WorldContextObject));
	//	if (LocalPlayerCharacter && LocalPlayerCharacter->IsValidObj())
	//	{
	//		return bForMain ? LocalPlayerCharacter->GetCurrentPlayerClass() : LocalPlayerCharacter->GetTagPlayerClass();
	//	}
	//}
	// 현재 플레이하고 있는 LocalPlayerCharacter 가 들고 있는 값과 다를 수 있으므로 처음부터 LocalCharacterData 에 있는 걸 쓰지 않는다.
	// LocalPlayerCharacter 를 얻어올 수 없는 상황에서만.. 그런 상황이 있긴 함.
	return bForMain ? BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass() : BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass();
}

int32 UB2DialogCharacterInfoSet::GetInfoArrayIndexOfCharCode(FName CharacterCodeName, UObject* WorldContextObject)
{
	int32* ArrayIndex = DialogCharacterMap.Find(CharacterCodeName);
	if (ArrayIndex)
	{
		return *ArrayIndex;
	}

	// DialogCharacterMap 에서 못 찾은 경우 DialogPlayerClassMap 에서 찾는다. 현재 플레이 캐릭터에 따른 index
	if (CharacterCodeName == DIALOG_CHAR_USERMAINCLASS || CharacterCodeName == DIALOG_CHAR_USERSUBCLASS)
	{
		if (CharacterCodeName == DIALOG_CHAR_USERMAINCLASS)
			ArrayIndex = DialogPlayerClassMap.Find(LocalGetPlayerCharClass(WorldContextObject, true));
		else
			ArrayIndex = DialogPlayerClassMap.Find(LocalGetPlayerCharClass(WorldContextObject, false));
	}
	return ArrayIndex ? *ArrayIndex : INDEX_NONE;
}

FDialogCharacterInfo* UB2DialogCharacterInfoSet::GetDialogCharacterInfo(FName CharacterCodeName, UObject* WorldContextObject)
{
	// 이걸로 FDialogCharacterInfo 를 얻어오더라도 Matrial 이나 Texture 는 FDialogCharacterInfo 를 가지고 막바로 꺼내오기보다는 여기서 제공하는 다른 메쏘드를 써야 할 것.

	int32 InfoArrayIndex = GetInfoArrayIndexOfCharCode(CharacterCodeName, WorldContextObject);

	FDialogCharacterInfo* RetInfo = DialogCharacterInfoArray.IsValidIndex(InfoArrayIndex) ? &DialogCharacterInfoArray[InfoArrayIndex] : NULL;
#if WITH_EDITOR && !PLATFORM_MAC
	if (GIsEditor && !RetInfo)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("캐릭터 코드이름 %s 에 해당하는 DialogCharacterInfo 를 찾지 못함. 컴퓨터가 곧 폭발한다."), *CharacterCodeName.ToString())
			));
	}
#endif
	return RetInfo;
}

UMaterialInstance* UB2DialogCharacterInfoSet::GetDialogCharacterPortraitMtrl(FName CharacterCodeName, EPortraitType InType, UObject* WorldContextObject)
{
	// LoadedRefHolder 를 제공해야 하므로 여기 밖에서 FDialogCharacterInfo 를 가지고 GetPortraitMtrlForType 하지는 못할 꺼고 이걸로 사용해야.
	FDialogCharacterInfo* FoundInfo = GetDialogCharacterInfo(CharacterCodeName, WorldContextObject);
	if (FoundInfo)
	{
		int32 InfoArrayIndex = GetInfoArrayIndexOfCharCode(CharacterCodeName, WorldContextObject);
		FDialogCharacterInfoLoadedRefHolder& FoundOrAddHolder = LoadedResourceHolder.FindOrAdd(InfoArrayIndex);

		return FoundInfo->GetPortraitMtrlForType(InfoLoadManager, InType, FoundOrAddHolder);
	}
	return nullptr;
}
UTexture2D* UB2DialogCharacterInfoSet::GetWholeBodyPortrait(FName CharacterCodeName, UObject* WorldContextObject)
{
	// LoadedRefHolder 를 제공해야 하므로 여기 밖에서 FDialogCharacterInfo 를 가지고 GetWholeBodyPortrait 하지는 못할 꺼고 이걸로 사용해야.
	FDialogCharacterInfo* FoundInfo = GetDialogCharacterInfo(CharacterCodeName, WorldContextObject);
	if (FoundInfo)
	{
		int32 InfoArrayIndex = GetInfoArrayIndexOfCharCode(CharacterCodeName, WorldContextObject);
		FDialogCharacterInfoLoadedRefHolder& FoundOrAddHolder = LoadedResourceHolder.FindOrAdd(InfoArrayIndex);

		return FoundInfo->GetWholeBodyPortrait(InfoLoadManager, FoundOrAddHolder);
	}
	return nullptr;
}

void UB2DialogCharacterInfoSet::UnloadAll()
{
	for (int32 II = 0; II < DialogCharacterInfoArray.Num(); ++II)
	{
		FDialogCharacterInfo& ThisInfoArray = DialogCharacterInfoArray[II];
		// 여기선 FindOrAdd 할 필요없이 없으면 넘어가면 됨.
		FDialogCharacterInfoLoadedRefHolder* FoundHolder = LoadedResourceHolder.Find(II);
		if (FoundHolder)
		{
			ThisInfoArray.UnloadAll(InfoLoadManager, *FoundHolder);
		}
	}
	LoadedResourceHolder.Empty(); // 확인사실
}

#if WITH_EDITOR
void UB2DialogCharacterInfoSet::EditorLoadAll()
{
	for (int32 II = 0; II < DialogCharacterInfoArray.Num(); ++II)
	{
		FDialogCharacterInfo& ThisInfoArray = DialogCharacterInfoArray[II];
		FDialogCharacterInfoLoadedRefHolder& FoundOrAddHolder = LoadedResourceHolder.FindOrAdd(II);
		ThisInfoArray.EditorLoadAll(InfoLoadManager, FoundOrAddHolder);
	}
}
#endif
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2DialogCharacterInfoSet::CleanupOnPreSave()
{
	LoadedResourceHolder.Empty();
}
void UB2DialogCharacterInfoSet::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif

//======================================================================
// DialogSoundInfo, as separate lazy-loaded info asset table
//======================================================================

USoundCue* FDialogSoundInfo::GetSoundCue(FStreamableManager& InLoadManager, FSingleDialogSoundInfoLoadedRefHolder& InOutLoadedRefHolder)
{
	return GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, AssetPtr, &InOutLoadedRefHolder.LoadedSoundPtr, TEXT("DialogSoundInfo"));
}
void FDialogSoundInfo::Unload(FStreamableManager& InLoadManager, FSingleDialogSoundInfoLoadedRefHolder& InOutLoadedRefHolder)
{
	UnloadSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, AssetPtr, &InOutLoadedRefHolder.LoadedSoundPtr);
}

UB2DialogSoundInfoSet::UB2DialogSoundInfoSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TheData = NULL;
}

void UB2DialogSoundInfoSet::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	TheData = InDataTable;

#if WITH_EDITOR
	if (GIsEditor)
	{
		CheckInfoDataIntegrity();
	}
#endif
}

USoundCue* UB2DialogSoundInfoSet::GetInfoSound(FName InCharacterCodeName, FName InRawSoundCode, UObject* WorldContextObject)
{	
	if (!TheData){
		return NULL;
	}

	FDialogSoundInfo* FoundInfo = NULL;
	FName FinalDataKey = InRawSoundCode; // GC 방지용 LoadedPtrMap 에서의 키값인데 결국 DataTable 에서의 키값과 마찬가지로.

	if (InCharacterCodeName == DIALOG_CHAR_USERMAINCLASS || InCharacterCodeName == DIALOG_CHAR_USERSUBCLASS)
	{ // 플레이어 캐릭터의 경우는 현재 클래스 정보에 따라 적절한 suffix 를 붙여서 find
		FString FinalCodeString = InRawSoundCode.ToString();
		
		FString ClassSuffix = UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(
			(InCharacterCodeName == DIALOG_CHAR_USERMAINCLASS) ? LocalGetPlayerCharClass(WorldContextObject, true) : LocalGetPlayerCharClass(WorldContextObject, false)
		);
		FinalCodeString += (TEXT("_") + ClassSuffix);
		FinalDataKey = FName(*FinalCodeString);
		FoundInfo = TheData->FindRow<FDialogSoundInfo>(FinalDataKey, TEXT(""));
	}
	else
	{ // 기타의 경우는 별다른 프로세싱 없이 그냥 find.
		FoundInfo = TheData->FindRow<FDialogSoundInfo>(FinalDataKey, TEXT(""));
	}

	if (FoundInfo)
	{
		FSingleDialogSoundInfoLoadedRefHolder& FoundOrAddHolder = LoadedResourceHolder.FindOrAdd(FinalDataKey);

		USoundCue* RetObj = FoundInfo->GetSoundCue(InfoLoadManager, FoundOrAddHolder);
#if WITH_EDITOR && !PLATFORM_MAC
		if (!RetObj)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("DialogSoundInfo CharCode %s SoundCode %s 에 대한 Sound Asset 로딩 실패. 컴퓨터가 곧 폭발한다."), *InCharacterCodeName.ToString(), *InRawSoundCode.ToString())
				));
		}
#endif
		return RetObj;
	}
	return NULL;
}

void UB2DialogSoundInfoSet::UnloadAll()
{
	if (TheData)
	{
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			const FName& ThisRowKey = RowKeys[RI];
			FDialogSoundInfo* FoundDataRow = TheData->FindRow<FDialogSoundInfo>(ThisRowKey, TEXT(""));
			FSingleDialogSoundInfoLoadedRefHolder* FoundLoadedHolder = LoadedResourceHolder.Find(ThisRowKey);
			if (FoundDataRow && FoundLoadedHolder)
			{
				FoundDataRow->Unload(InfoLoadManager, *FoundLoadedHolder);
			}
		}
		LoadedResourceHolder.Empty(); // 확인 사살
	}
}

#if WITH_EDITOR
void UB2DialogSoundInfoSet::EditorLoadAll()
{
	// DataTable 기반의 TAsset 이라 EditorLoadAll 이 반드시 필요.
	if (TheData)
	{
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			const FName& ThisRowKey = RowKeys[RI];
			FDialogSoundInfo* FoundDataRow = TheData->FindRow<FDialogSoundInfo>(ThisRowKey, TEXT(""));
			if (FoundDataRow)
			{
				FSingleDialogSoundInfoLoadedRefHolder& FoundOrAddHolder = LoadedResourceHolder.FindOrAdd(ThisRowKey);
				FoundDataRow->GetSoundCue(InfoLoadManager, FoundOrAddHolder);
			}
		}
	}
}
void UB2DialogSoundInfoSet::CheckInfoDataIntegrity()
{
#if !PLATFORM_MAC
	if (!TheData)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("DialogSoundInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
			));
		return;
	}
#endif
	// 기타 필요한 체크들.. 중복된 ID 있는지 등.
}
#endif

//======================================================================
// The final dialog info, putting things together.
//======================================================================

UB2ScenarioDialogInfo::UB2ScenarioDialogInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FDialogLine::FDialogLine(struct FScenarioDialogInfoData* InfoData)
{
	check(InfoData);
	if (InfoData != NULL)
	{
		DialogLineCodeName	= InfoData->DialogLineCodeName;
		SpeakerCodeName		= InfoData->SpeakerCodeName;
		DialogSoundCodeName = InfoData->DialogSoundCodeName;
		PortraitType		= InfoData->PortraitType;
		ShowingTime			= InfoData->ShowingTime;
		DelayToNextLine		= InfoData->DelayToNextLine;
	}
}

FDialogInfoInstance::FDialogInfoInstance(struct FScenarioDialogInfoData* InfoData)
{
	check(InfoData);
	if (InfoData != NULL)
	{
		DialogCodeName = InfoData->DialogCodeName;
		bNeedPauseGame = InfoData->NeedPauseGame;
		DialogSequence.Add(FDialogLine(InfoData));
	}
}

void UB2ScenarioDialogInfo::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	DialogInfoDataTable = InDataTable;

	DialogInfoMap.Empty(DialogInfoDataTable->GetRowMap().Num());

	for (TMap<FName, uint8*>::TConstIterator It(DialogInfoDataTable->GetRowMap()); It; ++It)
	{
		auto* DialogInfoData = reinterpret_cast<FScenarioDialogInfoData*>(It.Value());
		if (DialogInfoData)
		{
			auto* InstancePtr = DialogInfoMap.Find(DialogInfoData->DialogCodeName);
			if (InstancePtr)
			{
				InstancePtr->DialogSequence.Add(FDialogLine(DialogInfoData));
			}
			else
			{
				DialogInfoMap.Add(DialogInfoData->DialogCodeName, FDialogInfoInstance(DialogInfoData));
			}			
		}
	}
}

const FDialogInfoInstance* UB2ScenarioDialogInfo::GetDialogInfo(FName DialogCodeName)
{
	return DialogInfoMap.Find(DialogCodeName);
}

void UB2ScenarioDialogInfo::PreloadAssetsForStageGame(const TArray<FName>& InPreloadDialogNames, UObject* WorldContextObject, bool bIncludeWholeBodyPortrait)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2ScenarioDialogInfo::PreloadAssetsForStageGame, %d Dialogs"), InPreloadDialogNames.Num()));

	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	//UB2DialogCharacterInfoSet* DlgCharacterInfoSet = B2GI ? B2GI->GetDialogCharacterInfoTable() : StaticFindDialogCharacterInfoTable();
	//UB2DialogSoundInfoSet* DlgSoundInfoSet = StaticFindDialogSoundInfoTable();
	//
	//// 스테이지 로딩 시점에 필요한 dialog 에셋들을 미리 로딩하기 위함.

	//if (DlgCharacterInfoSet && DlgSoundInfoSet)
	//{
	//	for (const FName& ThisDlgCode : InPreloadDialogNames)
	//	{
	//		const FDialogInfoInstance* DlgInfoOfCode = GetDialogInfo(ThisDlgCode);
	//		if (DlgInfoOfCode)
	//		{
	//			for (const FDialogLine& ThisLineInfo : DlgInfoOfCode->DialogSequence)
	//			{
	//				DlgCharacterInfoSet->GetDialogCharacterPortraitMtrl(ThisLineInfo.SpeakerCodeName, ThisLineInfo.PortraitType, WorldContextObject);
	//				if (bIncludeWholeBodyPortrait){
	//					DlgCharacterInfoSet->GetWholeBodyPortrait(ThisLineInfo.SpeakerCodeName, WorldContextObject);
	//				}
	//				DlgSoundInfoSet->GetInfoSound(ThisLineInfo.SpeakerCodeName, ThisLineInfo.DialogSoundCodeName, WorldContextObject);
	//				
	//				// 메인/태그가 언제 바뀔지 모르므로 둘 다 로딩하도록 해야..
	//				FName AdditionalSpeakerCodeName = NAME_None;
	//				if (ThisLineInfo.SpeakerCodeName == DIALOG_CHAR_USERMAINCLASS)
	//				{
	//					AdditionalSpeakerCodeName = DIALOG_CHAR_USERSUBCLASS;
	//				}
	//				else if (ThisLineInfo.SpeakerCodeName == DIALOG_CHAR_USERSUBCLASS)
	//				{
	//					AdditionalSpeakerCodeName = DIALOG_CHAR_USERMAINCLASS;
	//				}

	//				if (AdditionalSpeakerCodeName != NAME_None)
	//				{
	//					DlgCharacterInfoSet->GetDialogCharacterPortraitMtrl(AdditionalSpeakerCodeName, ThisLineInfo.PortraitType, WorldContextObject);
	//					if (bIncludeWholeBodyPortrait){
	//						DlgCharacterInfoSet->GetWholeBodyPortrait(AdditionalSpeakerCodeName, WorldContextObject);
	//					}
	//					DlgSoundInfoSet->GetInfoSound(AdditionalSpeakerCodeName, ThisLineInfo.DialogSoundCodeName, WorldContextObject);
	//				}
	//			}
	//		}
	//	}
	//}
}