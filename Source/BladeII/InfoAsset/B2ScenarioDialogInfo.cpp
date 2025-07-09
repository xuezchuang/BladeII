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
			{ // LoadedPortraitMtrls �� �ִ� �� ������ Ű �˻��ϰ� �ϸ� �� �����ϹǷ�.. �̰� AddUnique ��
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
		UnloadSomeLazyLoadedAssetCommon<UMaterialInstance>(InLoadManager, ThisPortraitInfo.Portrait, &DummyMtrlPtr); // LoadedPtrCache �� �Ʒ��� ���� ó��.
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
		// �ȿ��� �ٺ����� for ���� �� ���� �Ǵµ�.. ���� �� �ƴϰ� ������ ����̴� ����.
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
	// �ణ Ư���� �����ε� DialogCharacterInfoArray ��ü�� TMap �̳� �׷� ���� �ĺ��ڿ� ���� �����ִ� Ÿ���� �ƴϰ�
	// PostLoad ������ Array ���� index ���� �̸� ĳ���� ��.
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
	// ���� �÷����ϰ� �ִ� LocalPlayerCharacter �� ��� �ִ� ���� �ٸ� �� �����Ƿ� ó������ LocalCharacterData �� �ִ� �� ���� �ʴ´�.
	// LocalPlayerCharacter �� ���� �� ���� ��Ȳ������.. �׷� ��Ȳ�� �ֱ� ��.
	return bForMain ? BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass() : BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass();
}

int32 UB2DialogCharacterInfoSet::GetInfoArrayIndexOfCharCode(FName CharacterCodeName, UObject* WorldContextObject)
{
	int32* ArrayIndex = DialogCharacterMap.Find(CharacterCodeName);
	if (ArrayIndex)
	{
		return *ArrayIndex;
	}

	// DialogCharacterMap ���� �� ã�� ��� DialogPlayerClassMap ���� ã�´�. ���� �÷��� ĳ���Ϳ� ���� index
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
	// �̰ɷ� FDialogCharacterInfo �� �������� Matrial �̳� Texture �� FDialogCharacterInfo �� ������ ���ٷ� �������⺸�ٴ� ���⼭ �����ϴ� �ٸ� �޽�带 ��� �� ��.

	int32 InfoArrayIndex = GetInfoArrayIndexOfCharCode(CharacterCodeName, WorldContextObject);

	FDialogCharacterInfo* RetInfo = DialogCharacterInfoArray.IsValidIndex(InfoArrayIndex) ? &DialogCharacterInfoArray[InfoArrayIndex] : NULL;
#if WITH_EDITOR && !PLATFORM_MAC
	if (GIsEditor && !RetInfo)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ĳ���� �ڵ��̸� %s �� �ش��ϴ� DialogCharacterInfo �� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."), *CharacterCodeName.ToString())
			));
	}
#endif
	return RetInfo;
}

UMaterialInstance* UB2DialogCharacterInfoSet::GetDialogCharacterPortraitMtrl(FName CharacterCodeName, EPortraitType InType, UObject* WorldContextObject)
{
	// LoadedRefHolder �� �����ؾ� �ϹǷ� ���� �ۿ��� FDialogCharacterInfo �� ������ GetPortraitMtrlForType ������ ���� ���� �̰ɷ� ����ؾ�.
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
	// LoadedRefHolder �� �����ؾ� �ϹǷ� ���� �ۿ��� FDialogCharacterInfo �� ������ GetWholeBodyPortrait ������ ���� ���� �̰ɷ� ����ؾ�.
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
		// ���⼱ FindOrAdd �� �ʿ���� ������ �Ѿ�� ��.
		FDialogCharacterInfoLoadedRefHolder* FoundHolder = LoadedResourceHolder.Find(II);
		if (FoundHolder)
		{
			ThisInfoArray.UnloadAll(InfoLoadManager, *FoundHolder);
		}
	}
	LoadedResourceHolder.Empty(); // Ȯ�λ��
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
	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
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
	FName FinalDataKey = InRawSoundCode; // GC ������ LoadedPtrMap ������ Ű���ε� �ᱹ DataTable ������ Ű���� ����������.

	if (InCharacterCodeName == DIALOG_CHAR_USERMAINCLASS || InCharacterCodeName == DIALOG_CHAR_USERSUBCLASS)
	{ // �÷��̾� ĳ������ ���� ���� Ŭ���� ������ ���� ������ suffix �� �ٿ��� find
		FString FinalCodeString = InRawSoundCode.ToString();
		
		FString ClassSuffix = UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(
			(InCharacterCodeName == DIALOG_CHAR_USERMAINCLASS) ? LocalGetPlayerCharClass(WorldContextObject, true) : LocalGetPlayerCharClass(WorldContextObject, false)
		);
		FinalCodeString += (TEXT("_") + ClassSuffix);
		FinalDataKey = FName(*FinalCodeString);
		FoundInfo = TheData->FindRow<FDialogSoundInfo>(FinalDataKey, TEXT(""));
	}
	else
	{ // ��Ÿ�� ���� ���ٸ� ���μ��� ���� �׳� find.
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
				FString::Printf(TEXT("DialogSoundInfo CharCode %s SoundCode %s �� ���� Sound Asset �ε� ����. ��ǻ�Ͱ� �� �����Ѵ�."), *InCharacterCodeName.ToString(), *InRawSoundCode.ToString())
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
		LoadedResourceHolder.Empty(); // Ȯ�� ���
	}
}

#if WITH_EDITOR
void UB2DialogSoundInfoSet::EditorLoadAll()
{
	// DataTable ����� TAsset �̶� EditorLoadAll �� �ݵ�� �ʿ�.
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
			FString::Printf(TEXT("DialogSoundInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
		return;
	}
#endif
	// ��Ÿ �ʿ��� üũ��.. �ߺ��� ID �ִ��� ��.
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
	//// �������� �ε� ������ �ʿ��� dialog ���µ��� �̸� �ε��ϱ� ����.

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
	//				// ����/�±װ� ���� �ٲ��� �𸣹Ƿ� �� �� �ε��ϵ��� �ؾ�..
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