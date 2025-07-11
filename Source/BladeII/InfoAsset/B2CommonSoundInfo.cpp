// Fill out your copyright notice in the Description page of Project Settings.


#include "B2CommonSoundInfo.h"

#include "Sound/SoundNode.h"

#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"
#include "UObject/ObjectSaveContext.h"

USoundCue* FSingleCommonSoundInfoMap::GetSoundCue(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap)
{
	// �ϴ� �ε��� ���� ĳ�� ����ü���� Ȯ��.
	FSingleCommonSoundInfoLoadedRefHolder& FoundOrAddEntry = LoadedPtrMap.FindOrAdd(MyArrayIndex);
	return GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, AssetPtr, &(FoundOrAddEntry.LoadedSoundPtr), TEXT("CommonSoundInfo"));
}

void FSingleCommonSoundInfoMap::Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap)
{
	LoadedPtrMap.Remove(MyArrayIndex);

	USoundCue* DummyLoadePtr = NULL; // ���� UnloadSomeLazyLoadedClassAssetCommon �� �Ѱ��ֱ� ����. ���⼱ ������ ���� ����.
	UnloadSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, AssetPtr, &DummyLoadePtr);
}

bool FSingleCommonSoundInfoMap_Stage::IsSupportedStageCombatContext(EStageCombatSoundContext InCheckContext)
{
	if (InCheckContext == EStageCombatSoundContext::ESCSC_NormalCombat){
		return bForNormalCombat;
	}
	else if (InCheckContext == EStageCombatSoundContext::ESCSC_BossCombat){
		return bForBossCombat;
	}
	return false;
}

void FSingleStageMiddleBossBGM::Unload(FStreamableManager& InLoadManager)
{
	USoundWave* DummyLoadePtr = NULL; // ���� UnloadSomeLazyLoadedClassAssetCommon �� �Ѱ��ֱ� ����. ���⼱ ������ ���� ����.
	UnloadSomeLazyLoadedAssetCommon<USoundWave>(InLoadManager, AssetPtr, &DummyLoadePtr);
}

USoundWave* FSingleStageMiddleBossBGM::GetSoundWave(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleSoundWaveInfoLoadedRefHolder>& LoadedPtrMap)
{
	FSingleSoundWaveInfoLoadedRefHolder* FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	if (!FoundLoadedPtrEntry)
	{
		FSingleSoundWaveInfoLoadedRefHolder NewLoadedEntry;
		LoadedPtrMap.Add(MyArrayIndex, NewLoadedEntry);
		FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	}
	checkSlow(FoundLoadedPtrEntry);

	return GetSomeLazyLoadedAssetCommon<USoundWave>(InLoadManager, AssetPtr, &(FoundLoadedPtrEntry->LoadedSoundPtr), TEXT("CommonSoundInfo"));
}

USoundCue* FStageMiddleBossBGM::GetSoundCue(FStreamableManager& InLoadManager)
{
	USoundCue* TemplateCue = GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, SoundCuePtr, &(LoadedSoundHolder.LoadedSoundPtr), TEXT("CommonSoundInfo"));

	return DuplicateObject<USoundCue>(TemplateCue, GetTransientPackage(), NAME_None);
}

void FStageMiddleBossBGM::UnloadAll(FStreamableManager& InLoadManager, TMap<int32, FSingleSoundWaveInfoLoadedRefHolder>& LoadedPtrMap)
{
	LoadedPtrMap.Remove(0);
	LoadedPtrMap.Remove(1);

	for (auto &Each : IntroBGMArray)
	{
		if (Each.AssetPtr.IsValid())
		{
			Each.Unload(InLoadManager);
		}
	}

	for (auto &Each : LoopBGMArray)
	{
		if (Each.AssetPtr.IsValid())
		{
			Each.Unload(InLoadManager);
		}
	}

	LoadedSoundHolder.LoadedSoundPtr = nullptr;
	USoundCue* DummyLoadePtr = nullptr;
	UnloadSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, SoundCuePtr, &DummyLoadePtr);
}

UB2CommonSoundInfo::UB2CommonSoundInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2CommonSoundInfo::PostLoad()
{
#if WITH_EDITOR
	EnsureDataIntegrity();
#endif
	Super::PostLoad();
}

USoundCue* UB2CommonSoundInfo::GetSoundByID(ECommonSoundID InID)
{
	FSingleCommonSoundInfoMap* FoundData = MainInfoMap.Find(InID);
	if (FoundData)
	{
		checkSlow(FoundData->SoundID == InID);
		// ���� ĳ�� Ű ���� �ۿ��� �Կ��� �ʿ䰡 ������ �ٸ� ��Ȳ������ ����ϴ� �׳� �̷��� ��.
		return FoundData->GetSoundCue(InfoLoadManager, CommonSoundIDToInt(InID), LoadedMainInfoPtrMap);
	}
	return NULL;
}

float UB2CommonSoundInfo::GetFadeInOutDuractionByID(ECommonSoundID InID)
{
	FSingleCommonSoundInfoMap* FoundData = MainInfoMap.Find(InID);
	if (FoundData)
		return FoundData->FadeInOutDuration;

	return 0.0f;
}

bool UB2CommonSoundInfo::GetSoundByUIScene(EUIScene InUIScene, USoundCue** OutSoundCue, float & OutFadeInOutDuration)
{
	FSingleCommonSoundInfoMap_UIScene* FoundData = UISceneMap.Find(InUIScene);
	if (FoundData)
	{
		if(OutSoundCue)
			(*OutSoundCue) = FoundData->GetSoundCue(InfoLoadManager, static_cast<int32>(InUIScene), LoadedUISceneBGMPtrMap);
		OutFadeInOutDuration = FoundData->FadeInOutDuration;
		return true;
	}
	return false;
}

USoundCue* UB2CommonSoundInfo::GetBGMSoundOfStageCombat(int32 InStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, float& OutFadeInOutDuration)
{
	bool IsBossCombat = InStageContext == EStageCombatSoundContext::ESCSC_BossCombat;
	bool IsMiddleBoss = (IsBossCombat == true && bIsExtraMap == false);
	
	OutFadeInOutDuration = 0.0f;
	// �̵� ���� BGM ���� ó��
	if (IsMiddleBoss)
	{
		return GetMissleBossStageBGM(OutFadeInOutDuration);
	}
	else
	{
		return GetNormalOrBossStageBGM(InStageId, bIsExtraMap, InStageContext, OutFadeInOutDuration);
	}
	return NULL;
}


USoundCue* UB2CommonSoundInfo::GetMissleBossStageBGM(float& OutFadeInOutDuration)
{
	if (StageMiddleBGM.IntroBGMArray.Num() < 1 || StageMiddleBGM.LoopBGMArray.Num() < 1)
	{
		ensure(!TEXT("CommonSoundInfo Middleboss BGM CHECK!"));
		return nullptr;
	}

	const int32 RandIntroIndex = FMath::RandRange(0, StageMiddleBGM.IntroBGMArray.Num() - 1);
	const int32 RandLoopIndex = FMath::RandRange(0, StageMiddleBGM.LoopBGMArray.Num() - 1);

	USoundWave *SoundWaveA = StageMiddleBGM.IntroBGMArray[RandIntroIndex].GetSoundWave(InfoLoadManager, 0, LoadedMiddleStageBGMPtrMap);
	USoundWave *SoundWaveB = StageMiddleBGM.LoopBGMArray[RandLoopIndex].GetSoundWave(InfoLoadManager, 1, LoadedMiddleStageBGMPtrMap);

	USoundCue *BGMSoundCue = StageMiddleBGM.GetSoundCue(InfoLoadManager);
	if (BGMSoundCue == nullptr)
	{
		return nullptr;
	}
	if (CreateSoundCueConcatenator(BGMSoundCue, SoundWaveA, SoundWaveB))
	{
		OutFadeInOutDuration = StageMiddleBGM.FadeInOutDuration;
		return BGMSoundCue;
	}
	return nullptr;
}

USoundCue* UB2CommonSoundInfo::GetCustomizeBossBGM(const FName& InCoustomName)
{
	if (!StageCustomizeBossBGM.Num() && !StageCustomizeBossBGM.Find(InCoustomName))
	{
		ensure(!TEXT("CommonSoundInfo CustomizeBossBGM BGM CHECK!"));
		return nullptr;
	}

	return StageCustomizeBossBGM[InCoustomName].GetSoundCue(InfoLoadManager, LoadedCoutomizeMap, InCoustomName);

}

USoundCue* UB2CommonSoundInfo::GetNormalOrBossStageBGM(int32 InStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, float& OutFadeInOutDuration,int32 InWaveNumberbool, const FName& InName)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2CommonSoundInfo::GetNormalOrBossStageBGM Stage%d"), InStageId));

	// ���� InfoArray_StageBGM ��Ҹ� RelevantStageKeyStr �ܿ� ������ �ĺ��� �� �ִ� �������� ������ ���ϸ� ©���� ��� ��ҿ� ���� iteration �� ���ƾ� ��.
	// ��ü�� �װͺ��� ���ҽ� �ε��� �ð��� �� ��Ƹ԰�����. 
	for (int32 SII = 0; SII < InfoArray_StageBGM.Num(); ++SII)
	{
		FSingleCommonSoundInfoMap_Stage& CurrInfo = InfoArray_StageBGM[SII];

		// ���⼱ SoundID ��� StageId �� ��Ȳ�� �˻�. 

		// ���� ���� �˻���� ����.
		if (CurrInfo.bForExtraMap == bIsExtraMap && CurrInfo.IsSupportedStageCombatContext(InStageContext))
		{
			if (CurrInfo.CachedSupportedStageList.Num() == 0)
			{ // �ѹ� �Ľ��ϰ� �� �Ŀ��� ������ ��԰� ĳ���صд�. ��Ÿ�ӿ��� �δ㽺���� ���� �ִ� ���ڿ� �Ľ��ε��ٰ� ��� CachedSupportedStageList ����� �ſ� ū ��쵵 �ִ�.
				ParseStageKeyStr(CurrInfo.CachedSupportedStageList, CurrInfo.RelevantStageKeyStr);
			}

			// CachedSupportedStageList �� ������ ���� �ѵΰ� ���� ���� ���� ������ ���� �̻��� �ſ� ���� element �� ������ �� ���� �ִ�. ���� TMap ���� ���.
			const bool bStageIDMatch = (CurrInfo.CachedSupportedStageList.Find(InStageId) != nullptr);

#if WITH_EDITOR
			if (GIsEditor)
			{ // �����Ϳ��� RelevantStageKeyStr �� ������ �� ������ �Ź� ���� ���� ���.
				CurrInfo.CachedSupportedStageList.Empty();
			}
#endif
			if (bStageIDMatch)
			{ // �ƹ��� ������ ���ڿ� �Ľ��ϰ� ������ �ص� �������ؼ� ������ �ð��� �Դ� �� �̰� ��

				
				OutFadeInOutDuration = CurrInfo.FadeInOutDuration;
				return CurrInfo.GetSoundCue(InfoLoadManager, SII, LoadedStageBGMPtrMap);
		
			}
		}


	}
	return NULL;
}

USoundCue* UB2CommonSoundInfo::GetButtonClickDefSound(int32 InIndex)
{
	// ButtonClickDefSoundArray �� int �ε����� Ű�� ����ϱ�� �Ͽ���.
	if (ButtonClickDefSoundArray.IsValidIndex(InIndex))
	{
		return ButtonClickDefSoundArray[InIndex].AssetPtr;
	}
	return NULL;
}

void UB2CommonSoundInfo::ParseStageKeyStr(TMap<int32, bool>& OutStages, const FString& InKeyStr)
{
	// ���� ��� "5,8-10" �̸� OutStages �� 5,8,9,10 �� ���� ��.

	check(OutStages.Num() == 0); // ����ִ� �� �־��� ��.

	TArray<FString> CommaSeparated;
	// �ϴ� ��ǥ �������� ������.
	InKeyStr.ParseIntoArray(CommaSeparated, TEXT(","));

	for (FString& CurrCommaSeparated : CommaSeparated)
	{
		// �� �и��� ��ū �������� - ���� ���� ����.
		TArray<FString> HypenSeparated;

		CurrCommaSeparated.ParseIntoArray(HypenSeparated, TEXT("-"));

		if (HypenSeparated.Num() == 1) // ���� ���� ���� �ϳ��� ������ ���
		{
			int32 ThisParsedStage = FCString::Atoi(*HypenSeparated[0]);
			if (ThisParsedStage > 0){
				OutStages.Add(ThisParsedStage, true); // Ű���� �ǹ̰� �ִ�. Value �� ������ �־��ִ� ��.
			}
		}
		else if (HypenSeparated.Num() > 1) // �Ϲ����� ���� ����.
		{
			int32 ParsedMinStage = FCString::Atoi(*HypenSeparated[0]);
			int32 ParsedMaxStage = FCString::Atoi(*HypenSeparated[1]);
			if (ParsedMinStage > 0 && ParsedMaxStage > 0 && ParsedMaxStage >= ParsedMinStage){
				// Min ~ Max ���̰� �� �������� ��ĩ �ʹ� ���� �� ���� �ְڴ�..
				for (int32 SupportStage = ParsedMinStage; SupportStage <= ParsedMaxStage; ++SupportStage){
					OutStages.Add(SupportStage, true); // Ű���� �ǹ̰� �ִ�. Value �� ������ �־��ִ� ��.
				}
			}
		}
	}
}

void UB2CommonSoundInfo::PreloadClassAssets(const TArray<ECommonSoundID>& InIDs)
{
	for (ECommonSoundID ThisID : InIDs)
	{
		// AssetPtr �� valid ���� �ʴٸ� �̸� �غ��� ���� �� ��.
		GetSoundByID(ThisID);
	}

	// �� �ʿ��ϸ� �߰�.. �̰� �ʼ����� �ͱ����� �ƴ� ��.
}

void UB2CommonSoundInfo::UnloadAll()
{
	//for (TMap<ECommonSoundID, FSingleCommonSoundInfoMap>::TIterator InfoMapIt(MainInfoMap); InfoMapIt; ++InfoMapIt)
	//{
	//	InfoMapIt.Value().Unload(InfoLoadManager, CommonSoundIDToInt(InfoMapIt.Key()), LoadedMainInfoPtrMap);
	//}
	//LoadedMainInfoPtrMap.Empty(); // Ȯ�λ��
	//for (int32 CII = 0; CII < InfoArray_StageBGM.Num(); ++CII)
	//{
	//	InfoArray_StageBGM[CII].Unload(InfoLoadManager, CII, LoadedStageBGMPtrMap);
	//}
	//LoadedStageBGMPtrMap.Empty(); // Ȯ�λ��

	//for (TMap<EUIScene, FSingleCommonSoundInfoMap_UIScene>::TIterator InfoMapIt(UISceneMap); InfoMapIt; ++InfoMapIt)
	//{
	//	InfoMapIt.Value().Unload(InfoLoadManager, static_cast<int32>(InfoMapIt.Key()), LoadedUISceneBGMPtrMap);
	//}
	//LoadedUISceneBGMPtrMap.Empty();

	//StageMiddleBGM.UnloadAll(InfoLoadManager, LoadedMiddleStageBGMPtrMap);
	//LoadedMiddleStageBGMPtrMap.Empty();
	//// ButtonClickDefSoundArray �� ��� �ƴ�.
}

#if WITH_EDITOR
void UB2CommonSoundInfo::EditorLoadAll()
{
	if (GIsEditor == false)
	{
		return;
	}

	// �ѹ� �̸� �ҷ��ָ� AssetPtr �� valid �� �� ��. ���� �� ���߿� �޸𸮴� ��â �԰���.
	for (TMap<ECommonSoundID, FSingleCommonSoundInfoMap>::TIterator InfoMapIt(MainInfoMap); InfoMapIt; ++InfoMapIt)
	{
		InfoMapIt.Value().GetSoundCue(InfoLoadManager, CommonSoundIDToInt(InfoMapIt.Key()), LoadedMainInfoPtrMap);
	}
	for (int32 CII = 0; CII < InfoArray_StageBGM.Num(); ++CII)
	{
		InfoArray_StageBGM[CII].GetSoundCue(InfoLoadManager, CII, LoadedStageBGMPtrMap);
	}

	for (TMap<EUIScene, FSingleCommonSoundInfoMap_UIScene>::TIterator InfoMapIt(UISceneMap); InfoMapIt; ++InfoMapIt)
	{
		InfoMapIt.Value().GetSoundCue(InfoLoadManager, static_cast<int32>(InfoMapIt.Key()), LoadedUISceneBGMPtrMap);
	}

	// ButtonClickDefSoundArray �� ��� �ƴ�.
}

void UB2CommonSoundInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_SoundID = GET_MEMBER_NAME_CHECKED(FSingleCommonSoundInfoMap, SoundID);
	const FName Name_MainInfoMap = GET_MEMBER_NAME_CHECKED(UB2CommonSoundInfo, MainInfoMap);
	const FName Name_RelevantStageKeyStr = GET_MEMBER_NAME_CHECKED(FSingleCommonSoundInfoMap_Stage, RelevantStageKeyStr);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_SoundID || PropertyName == Name_MainInfoMap)
	{
		EnsureDataIntegrity();
	}

#if !PLATFORM_MAC
	if (PropertyName == Name_RelevantStageKeyStr)
	{
		//FString WarnMsg = TEXT("���� StageBGM �׸��� RelevantStageKeyStr �� �ʹ� ���� ���������� �����ϰ� �ֽ��ϴ�. �̴� �������� ���� ���� ��Ұ� �ǹǷ� ��� ���α׷��ӿ��� ������ ������ ���� �����ϼ���.\r\n\r\n");
		FString WarnMsg = TEXT("StageBGM RelevantStageKeyStr \r\n");
		int32 FoundCount = 0;
		for (int32 SII = 0; SII < InfoArray_StageBGM.Num(); ++SII)
		{
			FSingleCommonSoundInfoMap_Stage& CurrInfo = InfoArray_StageBGM[SII];
			TMap<int32, bool> ParsedSupportedStages;
			ParseStageKeyStr(ParsedSupportedStages, CurrInfo.RelevantStageKeyStr);
			if (ParsedSupportedStages.Num() > 1000)
			{
				++FoundCount;
				WarnMsg += FString::Printf(TEXT("- No.%d,  Key Str: %s. %d"), SII, *CurrInfo.RelevantStageKeyStr, ParsedSupportedStages.Num());
			}
		}
		if (FoundCount > 0)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsg));
		}
	}
#endif
}

void UB2CommonSoundInfo::EnsureDataIntegrity()
{
	for (TMap<ECommonSoundID, FSingleCommonSoundInfoMap>::TIterator InfoMapIt(MainInfoMap); InfoMapIt; ++InfoMapIt)
	{
		ECommonSoundID ThisKey = InfoMapIt.Key();
		FSingleCommonSoundInfoMap& ThisInfoData = InfoMapIt.Value();
		// �� FSingleCommonSoundInfoMap ����� SoundID �� ��ϵ� �� Ű������ ������.
		if (ThisInfoData.SoundID != ThisKey)
		{
			MarkPackageDirty();
		}
		ThisInfoData.SoundID = ThisKey;
	}
}
#endif

// TAsset lazy-loading �� ����ϴ� Blueprint InfoAsset �� ������ ������ �������� �ذ��ϱ� �������� �ӽ� ����.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2CommonSoundInfo::CleanupOnPreSave()
{
	LoadedMainInfoPtrMap.Empty();
	LoadedStageBGMPtrMap.Empty();
	LoadedMiddleStageBGMPtrMap.Empty();
	LoadedUISceneBGMPtrMap.Empty();
}
void UB2CommonSoundInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif

USoundCue* FStageCustomizeBossBGM::GetSoundCue(FStreamableManager& InLoadManager, TMap<FName, FStageCustomizeBossBGMInfoLoadedRefHolder>& InLoadedStageBGMPtrMap, const FName& InName)
{
	FStageCustomizeBossBGMInfoLoadedRefHolder& SoundInfo = InLoadedStageBGMPtrMap.FindOrAdd(InName);
	USoundCue* TemplateCue = GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, SoundCuePtr, &(SoundInfo.LoadedSoundPtr), TEXT("CommonSoundInfo"));

	return TemplateCue;
}
