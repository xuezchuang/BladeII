
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
			{ // LoadedPortraitMtrls 俊 持绰 巴 鳖瘤绰 虐 八荤窍绊 窍搁 逞 汗棱窍骨肺.. 捞扒 AddUnique 肺
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
		UnloadSomeLazyLoadedAssetCommon<UMaterialInstance>(InLoadManager, ThisPortraitInfo.Portrait, &DummyMtrlPtr); // LoadedPtrCache 绰 酒贰辑 蝶肺 贸府.
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
		// 救俊辑 官焊鞍捞 for 巩阑 肚 倒霸 登绰单.. 缴茄 扒 酒聪绊 俊叼磐 扁瓷捞聪 毫淋.
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
	// 距埃 漂捞茄 备己牢单 DialogCharacterInfoArray 磊眉啊 TMap 捞唱 弊繁 侥狼 侥喊磊客 鞍捞 弓咯乐绰 鸥涝捞 酒聪绊
	// PostLoad 矫痢俊 Array 立辟 index 甸阑 固府 某教秦 狄.
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
	// 泅犁 敲饭捞窍绊 乐绰 LocalPlayerCharacter 啊 甸绊 乐绰 蔼苞 促甫 荐 乐栏骨肺 贸澜何磐 LocalCharacterData 俊 乐绰 吧 静瘤 臼绰促.
	// LocalPlayerCharacter 甫 掘绢棵 荐 绝绰 惑炔俊辑父.. 弊繁 惑炔捞 乐变 窃.
	//return bForMain ? BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass() : BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass();
	return EPCClass::EPC_End;
}

int32 UB2DialogCharacterInfoSet::GetInfoArrayIndexOfCharCode(FName CharacterCodeName, UObject* WorldContextObject)
{
	int32* ArrayIndex = DialogCharacterMap.Find(CharacterCodeName);
	if (ArrayIndex)
	{
		return *ArrayIndex;
	}

	// DialogCharacterMap 俊辑 给 茫篮 版快 DialogPlayerClassMap 俊辑 茫绰促. 泅犁 敲饭捞 某腐磐俊 蝶弗 index
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
	// 捞吧肺 FDialogCharacterInfo 甫 掘绢坷歹扼档 Matrial 捞唱 Texture 绰 FDialogCharacterInfo 甫 啊瘤绊 阜官肺 波郴坷扁焊促绰 咯扁辑 力傍窍绰 促弗 皋筋靛甫 结具 且 巴.

	int32 InfoArrayIndex = GetInfoArrayIndexOfCharCode(CharacterCodeName, WorldContextObject);

	FDialogCharacterInfo* RetInfo = DialogCharacterInfoArray.IsValidIndex(InfoArrayIndex) ? &DialogCharacterInfoArray[InfoArrayIndex] : NULL;
#if WITH_EDITOR && !PLATFORM_MAC
	if (GIsEditor && !RetInfo)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("某腐磐 内靛捞抚 %s 俊 秦寸窍绰 DialogCharacterInfo 甫 茫瘤 给窃. 哪腔磐啊 梆 气惯茄促."), *CharacterCodeName.ToString())
			));
	}
#endif
	return RetInfo;
}

UMaterialInstance* UB2DialogCharacterInfoSet::GetDialogCharacterPortraitMtrl(FName CharacterCodeName, EPortraitType InType, UObject* WorldContextObject)
{
	// LoadedRefHolder 甫 力傍秦具 窍骨肺 咯扁 观俊辑 FDialogCharacterInfo 甫 啊瘤绊 GetPortraitMtrlForType 窍瘤绰 给且 波绊 捞吧肺 荤侩秦具.
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
	// LoadedRefHolder 甫 力傍秦具 窍骨肺 咯扁 观俊辑 FDialogCharacterInfo 甫 啊瘤绊 GetWholeBodyPortrait 窍瘤绰 给且 波绊 捞吧肺 荤侩秦具.
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
		// 咯扁急 FindOrAdd 且 鞘夸绝捞 绝栏搁 逞绢啊搁 凳.
		FDialogCharacterInfoLoadedRefHolder* FoundHolder = LoadedResourceHolder.Find(II);
		if (FoundHolder)
		{
			ThisInfoArray.UnloadAll(InfoLoadManager, *FoundHolder);
		}
	}
	LoadedResourceHolder.Empty(); // 犬牢荤角
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
	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
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
	FName FinalDataKey = InRawSoundCode; // GC 规瘤侩 LoadedPtrMap 俊辑狼 虐蔼牢单 搬惫 DataTable 俊辑狼 虐蔼苞 付蛮啊瘤肺.

	if (InCharacterCodeName == DIALOG_CHAR_USERMAINCLASS || InCharacterCodeName == DIALOG_CHAR_USERSUBCLASS)
	{ // 敲饭捞绢 某腐磐狼 版快绰 泅犁 努贰胶 沥焊俊 蝶扼 利例茄 suffix 甫 嘿咯辑 find
		FString FinalCodeString = InRawSoundCode.ToString();
		
		FString ClassSuffix = UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(
			(InCharacterCodeName == DIALOG_CHAR_USERMAINCLASS) ? LocalGetPlayerCharClass(WorldContextObject, true) : LocalGetPlayerCharClass(WorldContextObject, false)
		);
		FinalCodeString += (TEXT("_") + ClassSuffix);
		FinalDataKey = FName(*FinalCodeString);
		FoundInfo = TheData->FindRow<FDialogSoundInfo>(FinalDataKey, TEXT(""));
	}
	else
	{ // 扁鸥狼 版快绰 喊促弗 橇肺技教 绝捞 弊成 find.
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
				FString::Printf(TEXT("DialogSoundInfo CharCode %s SoundCode %s 俊 措茄 Sound Asset 肺爹 角菩. 哪腔磐啊 梆 气惯茄促."), *InCharacterCodeName.ToString(), *InRawSoundCode.ToString())
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
		LoadedResourceHolder.Empty(); // 犬牢 荤混
	}
}

#if WITH_EDITOR
void UB2DialogSoundInfoSet::EditorLoadAll()
{
	// DataTable 扁馆狼 TAsset 捞扼 EditorLoadAll 捞 馆靛矫 鞘夸.
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
			FString::Printf(TEXT("DialogSoundInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
			));
		return;
	}
#endif
	// 扁鸥 鞘夸茄 眉农甸.. 吝汗等 ID 乐绰瘤 殿.
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
	//// 胶抛捞瘤 肺爹 矫痢俊 鞘夸茄 dialog 俊悸甸阑 固府 肺爹窍扁 困窃.

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
	//				// 皋牢/怕弊啊 攫力 官拆瘤 葛福骨肺 笛 促 肺爹窍档废 秦具..
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