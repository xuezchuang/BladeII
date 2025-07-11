// Fill out your copyright notice in the Description page of Project Settings.


#include "B2CommonSoundInfo.h"

#include "Sound/SoundNode.h"

#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"
#include "UObject/ObjectSaveContext.h"

USoundCue* FSingleCommonSoundInfoMap::GetSoundCue(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap)
{
	// 일단 로딩된 에셋 캐싱 구조체부터 확인.
	FSingleCommonSoundInfoLoadedRefHolder& FoundOrAddEntry = LoadedPtrMap.FindOrAdd(MyArrayIndex);
	return GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, AssetPtr, &(FoundOrAddEntry.LoadedSoundPtr), TEXT("CommonSoundInfo"));
}

void FSingleCommonSoundInfoMap::Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap)
{
	LoadedPtrMap.Remove(MyArrayIndex);

	USoundCue* DummyLoadePtr = NULL; // 단지 UnloadSomeLazyLoadedClassAssetCommon 에 넘겨주기 위함. 여기선 위에서 비우고 시작.
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
	USoundWave* DummyLoadePtr = NULL; // 단지 UnloadSomeLazyLoadedClassAssetCommon 에 넘겨주기 위함. 여기선 위에서 비우고 시작.
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
		// 여긴 캐싱 키 값을 밖에서 먹여줄 필요가 없지만 다른 상황에서도 사용하니 그냥 이렇게 둠.
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
	// 미들 보스 BGM 따로 처리
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

	// 여긴 InfoArray_StageBGM 요소를 RelevantStageKeyStr 외에 별도로 식별할 수 있는 기준으로 나누지 못하면 짤없이 모든 요소에 대해 iteration 을 돌아야 함.
	// 대체로 그것보단 리소스 로딩이 시간을 더 잡아먹겠지만. 
	for (int32 SII = 0; SII < InfoArray_StageBGM.Num(); ++SII)
	{
		FSingleCommonSoundInfoMap_Stage& CurrInfo = InfoArray_StageBGM[SII];

		// 여기선 SoundID 대신 StageId 와 상황을 검사. 

		// 보다 빠른 검사부터 먼저.
		if (CurrInfo.bForExtraMap == bIsExtraMap && CurrInfo.IsSupportedStageCombatContext(InStageContext))
		{
			if (CurrInfo.CachedSupportedStageList.Num() == 0)
			{ // 한번 파싱하고 난 후에는 다음에 써먹게 캐싱해둔다. 런타임에는 부담스러울 수도 있는 문자열 파싱인데다가 결과 CachedSupportedStageList 사이즈가 매우 큰 경우도 있다.
				ParseStageKeyStr(CurrInfo.CachedSupportedStageList, CurrInfo.RelevantStageKeyStr);
			}

			// CachedSupportedStageList 는 설정에 따라서 한두개 정도 뿐일 수도 있지만 수백 이상의 매우 많은 element 를 가지게 될 수도 있다. 따라서 TMap 으로 사용.
			const bool bStageIDMatch = (CurrInfo.CachedSupportedStageList.Find(InStageId) != nullptr);

#if WITH_EDITOR
			if (GIsEditor)
			{ // 에디터에선 RelevantStageKeyStr 이 편집될 수 있으니 매번 비우고 새로 계산.
				CurrInfo.CachedSupportedStageList.Empty();
			}
#endif
			if (bStageIDMatch)
			{ // 아무리 위에서 문자열 파싱하고 지랄을 해도 어지간해선 실제로 시간을 먹는 건 이거 ㅋ

				
				OutFadeInOutDuration = CurrInfo.FadeInOutDuration;
				return CurrInfo.GetSoundCue(InfoLoadManager, SII, LoadedStageBGMPtrMap);
		
			}
		}


	}
	return NULL;
}

USoundCue* UB2CommonSoundInfo::GetButtonClickDefSound(int32 InIndex)
{
	// ButtonClickDefSoundArray 는 int 인덱스를 키로 사용하기로 하였음.
	if (ButtonClickDefSoundArray.IsValidIndex(InIndex))
	{
		return ButtonClickDefSoundArray[InIndex].AssetPtr;
	}
	return NULL;
}

void UB2CommonSoundInfo::ParseStageKeyStr(TMap<int32, bool>& OutStages, const FString& InKeyStr)
{
	// 예를 들어 "5,8-10" 이면 OutStages 에 5,8,9,10 이 들어가야 함.

	check(OutStages.Num() == 0); // 비어있는 걸 넣어줄 것.

	TArray<FString> CommaSeparated;
	// 일단 쉼표 기준으로 나눈다.
	InKeyStr.ParseIntoArray(CommaSeparated, TEXT(","));

	for (FString& CurrCommaSeparated : CommaSeparated)
	{
		// 각 분리된 토큰 내에서는 - 으로 범위 지정.
		TArray<FString> HypenSeparated;

		CurrCommaSeparated.ParseIntoArray(HypenSeparated, TEXT("-"));

		if (HypenSeparated.Num() == 1) // 범위 지정 없이 하나만 지정한 경우
		{
			int32 ThisParsedStage = FCString::Atoi(*HypenSeparated[0]);
			if (ThisParsedStage > 0){
				OutStages.Add(ThisParsedStage, true); // 키값만 의미가 있다. Value 는 적당히 넣어주는 거.
			}
		}
		else if (HypenSeparated.Num() > 1) // 일반적인 범위 지정.
		{
			int32 ParsedMinStage = FCString::Atoi(*HypenSeparated[0]);
			int32 ParsedMaxStage = FCString::Atoi(*HypenSeparated[1]);
			if (ParsedMinStage > 0 && ParsedMaxStage > 0 && ParsedMaxStage >= ParsedMinStage){
				// Min ~ Max 사이가 넘 벌어지면 자칫 너무 많이 들어갈 수도 있겠다..
				for (int32 SupportStage = ParsedMinStage; SupportStage <= ParsedMaxStage; ++SupportStage){
					OutStages.Add(SupportStage, true); // 키값만 의미가 있다. Value 는 적당히 넣어주는 거.
				}
			}
		}
	}
}

void UB2CommonSoundInfo::PreloadClassAssets(const TArray<ECommonSoundID>& InIDs)
{
	for (ECommonSoundID ThisID : InIDs)
	{
		// AssetPtr 이 valid 하지 않다면 미리 준비해 놓게 될 것.
		GetSoundByID(ThisID);
	}

	// 꼭 필요하면 추가.. 이게 필수적일 것까지는 아닌 듯.
}

void UB2CommonSoundInfo::UnloadAll()
{
	//for (TMap<ECommonSoundID, FSingleCommonSoundInfoMap>::TIterator InfoMapIt(MainInfoMap); InfoMapIt; ++InfoMapIt)
	//{
	//	InfoMapIt.Value().Unload(InfoLoadManager, CommonSoundIDToInt(InfoMapIt.Key()), LoadedMainInfoPtrMap);
	//}
	//LoadedMainInfoPtrMap.Empty(); // 확인사살
	//for (int32 CII = 0; CII < InfoArray_StageBGM.Num(); ++CII)
	//{
	//	InfoArray_StageBGM[CII].Unload(InfoLoadManager, CII, LoadedStageBGMPtrMap);
	//}
	//LoadedStageBGMPtrMap.Empty(); // 확인사살

	//for (TMap<EUIScene, FSingleCommonSoundInfoMap_UIScene>::TIterator InfoMapIt(UISceneMap); InfoMapIt; ++InfoMapIt)
	//{
	//	InfoMapIt.Value().Unload(InfoLoadManager, static_cast<int32>(InfoMapIt.Key()), LoadedUISceneBGMPtrMap);
	//}
	//LoadedUISceneBGMPtrMap.Empty();

	//StageMiddleBGM.UnloadAll(InfoLoadManager, LoadedMiddleStageBGMPtrMap);
	//LoadedMiddleStageBGMPtrMap.Empty();
	//// ButtonClickDefSoundArray 는 대상 아님.
}

#if WITH_EDITOR
void UB2CommonSoundInfo::EditorLoadAll()
{
	if (GIsEditor == false)
	{
		return;
	}

	// 한번 미리 불러주면 AssetPtr 이 valid 해 질 것. 물론 이 와중에 메모리는 왕창 먹겠지.
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

	// ButtonClickDefSoundArray 는 대상 아님.
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
		//FString WarnMsg = TEXT("다음 StageBGM 항목의 RelevantStageKeyStr 이 너무 많은 스테이지를 포함하고 있습니다. 이는 잠재적인 성능 저하 요소가 되므로 담당 프로그래머에게 적절한 설정에 대해 문의하세요.\r\n\r\n");
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
		// 각 FSingleCommonSoundInfoMap 요소의 SoundID 는 등록된 맵 키값에서 가져옴.
		if (ThisInfoData.SoundID != ThisKey)
		{
			MarkPackageDirty();
		}
		ThisInfoData.SoundID = ThisKey;
	}
}
#endif

// TAsset lazy-loading 을 사용하는 Blueprint InfoAsset 의 오동작 문제를 정식으로 해결하기 전까지의 임시 땜질.
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

	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif

USoundCue* FStageCustomizeBossBGM::GetSoundCue(FStreamableManager& InLoadManager, TMap<FName, FStageCustomizeBossBGMInfoLoadedRefHolder>& InLoadedStageBGMPtrMap, const FName& InName)
{
	FStageCustomizeBossBGMInfoLoadedRefHolder& SoundInfo = InLoadedStageBGMPtrMap.FindOrAdd(InName);
	USoundCue* TemplateCue = GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, SoundCuePtr, &(SoundInfo.LoadedSoundPtr), TEXT("CommonSoundInfo"));

	return TemplateCue;
}
