// Fill out your copyright notice in the Description page of Project Settings.


#include "B2CommonSoundInfo.h"

#include "Sound/SoundNode.h"

#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"
#include "UObject/ObjectSaveContext.h"

USoundCue* FSingleCommonSoundInfoMap::GetSoundCue(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap)
{
	// 老窜 肺爹等 俊悸 某教 备炼眉何磐 犬牢.
	FSingleCommonSoundInfoLoadedRefHolder& FoundOrAddEntry = LoadedPtrMap.FindOrAdd(MyArrayIndex);
	return GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, AssetPtr, &(FoundOrAddEntry.LoadedSoundPtr), TEXT("CommonSoundInfo"));
}

void FSingleCommonSoundInfoMap::Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap)
{
	LoadedPtrMap.Remove(MyArrayIndex);

	USoundCue* DummyLoadePtr = NULL; // 窜瘤 UnloadSomeLazyLoadedClassAssetCommon 俊 逞败林扁 困窃. 咯扁急 困俊辑 厚快绊 矫累.
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
	USoundWave* DummyLoadePtr = NULL; // 窜瘤 UnloadSomeLazyLoadedClassAssetCommon 俊 逞败林扁 困窃. 咯扁急 困俊辑 厚快绊 矫累.
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
		// 咯变 某教 虐 蔼阑 观俊辑 冈咯临 鞘夸啊 绝瘤父 促弗 惑炔俊辑档 荤侩窍聪 弊成 捞犯霸 狄.
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
	// 固甸 焊胶 BGM 蝶肺 贸府
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

	// 咯变 InfoArray_StageBGM 夸家甫 RelevantStageKeyStr 寇俊 喊档肺 侥喊且 荐 乐绰 扁霖栏肺 唱穿瘤 给窍搁 漏绝捞 葛电 夸家俊 措秦 iteration 阑 倒酒具 窃.
	// 措眉肺 弊巴焊窜 府家胶 肺爹捞 矫埃阑 歹 棱酒冈摆瘤父. 
	for (int32 SII = 0; SII < InfoArray_StageBGM.Num(); ++SII)
	{
		FSingleCommonSoundInfoMap_Stage& CurrInfo = InfoArray_StageBGM[SII];

		// 咯扁急 SoundID 措脚 StageId 客 惑炔阑 八荤. 

		// 焊促 狐弗 八荤何磐 刚历.
		if (CurrInfo.bForExtraMap == bIsExtraMap && CurrInfo.IsSupportedStageCombatContext(InStageContext))
		{
			if (CurrInfo.CachedSupportedStageList.Num() == 0)
			{ // 茄锅 颇教窍绊 抄 饶俊绰 促澜俊 结冈霸 某教秦敌促. 繁鸥烙俊绰 何淬胶矾匡 荐档 乐绰 巩磊凯 颇教牢单促啊 搬苞 CachedSupportedStageList 荤捞令啊 概快 奴 版快档 乐促.
				ParseStageKeyStr(CurrInfo.CachedSupportedStageList, CurrInfo.RelevantStageKeyStr);
			}

			// CachedSupportedStageList 绰 汲沥俊 蝶扼辑 茄滴俺 沥档 挥老 荐档 乐瘤父 荐归 捞惑狼 概快 腹篮 element 甫 啊瘤霸 瞪 荐档 乐促. 蝶扼辑 TMap 栏肺 荤侩.
			const bool bStageIDMatch = (CurrInfo.CachedSupportedStageList.Find(InStageId) != nullptr);

#if WITH_EDITOR
			if (GIsEditor)
			{ // 俊叼磐俊急 RelevantStageKeyStr 捞 祈笼瞪 荐 乐栏聪 概锅 厚快绊 货肺 拌魂.
				CurrInfo.CachedSupportedStageList.Empty();
			}
#endif
			if (bStageIDMatch)
			{ // 酒公府 困俊辑 巩磊凯 颇教窍绊 瘤饿阑 秦档 绢瘤埃秦急 角力肺 矫埃阑 冈绰 扒 捞芭 せ

				
				OutFadeInOutDuration = CurrInfo.FadeInOutDuration;
				return CurrInfo.GetSoundCue(InfoLoadManager, SII, LoadedStageBGMPtrMap);
		
			}
		}


	}
	return NULL;
}

USoundCue* UB2CommonSoundInfo::GetButtonClickDefSound(int32 InIndex)
{
	// ButtonClickDefSoundArray 绰 int 牢郸胶甫 虐肺 荤侩窍扁肺 窍看澜.
	if (ButtonClickDefSoundArray.IsValidIndex(InIndex))
	{
		return ButtonClickDefSoundArray[InIndex].AssetPtr;
	}
	return NULL;
}

void UB2CommonSoundInfo::ParseStageKeyStr(TMap<int32, bool>& OutStages, const FString& InKeyStr)
{
	// 抗甫 甸绢 "5,8-10" 捞搁 OutStages 俊 5,8,9,10 捞 甸绢啊具 窃.

	check(OutStages.Num() == 0); // 厚绢乐绰 吧 持绢临 巴.

	TArray<FString> CommaSeparated;
	// 老窜 桨钎 扁霖栏肺 唱传促.
	InKeyStr.ParseIntoArray(CommaSeparated, TEXT(","));

	for (FString& CurrCommaSeparated : CommaSeparated)
	{
		// 阿 盒府等 配奴 郴俊辑绰 - 栏肺 裹困 瘤沥.
		TArray<FString> HypenSeparated;

		CurrCommaSeparated.ParseIntoArray(HypenSeparated, TEXT("-"));

		if (HypenSeparated.Num() == 1) // 裹困 瘤沥 绝捞 窍唱父 瘤沥茄 版快
		{
			int32 ThisParsedStage = FCString::Atoi(*HypenSeparated[0]);
			if (ThisParsedStage > 0){
				OutStages.Add(ThisParsedStage, true); // 虐蔼父 狼固啊 乐促. Value 绰 利寸洒 持绢林绰 芭.
			}
		}
		else if (HypenSeparated.Num() > 1) // 老馆利牢 裹困 瘤沥.
		{
			int32 ParsedMinStage = FCString::Atoi(*HypenSeparated[0]);
			int32 ParsedMaxStage = FCString::Atoi(*HypenSeparated[1]);
			if (ParsedMinStage > 0 && ParsedMaxStage > 0 && ParsedMaxStage >= ParsedMinStage){
				// Min ~ Max 荤捞啊 逞 国绢瘤搁 磊末 呈公 腹捞 甸绢哎 荐档 乐摆促..
				for (int32 SupportStage = ParsedMinStage; SupportStage <= ParsedMaxStage; ++SupportStage){
					OutStages.Add(SupportStage, true); // 虐蔼父 狼固啊 乐促. Value 绰 利寸洒 持绢林绰 芭.
				}
			}
		}
	}
}

void UB2CommonSoundInfo::PreloadClassAssets(const TArray<ECommonSoundID>& InIDs)
{
	for (ECommonSoundID ThisID : InIDs)
	{
		// AssetPtr 捞 valid 窍瘤 臼促搁 固府 霖厚秦 初霸 瞪 巴.
		GetSoundByID(ThisID);
	}

	// 怖 鞘夸窍搁 眠啊.. 捞霸 鞘荐利老 巴鳖瘤绰 酒囱 淀.
}

void UB2CommonSoundInfo::UnloadAll()
{
	//for (TMap<ECommonSoundID, FSingleCommonSoundInfoMap>::TIterator InfoMapIt(MainInfoMap); InfoMapIt; ++InfoMapIt)
	//{
	//	InfoMapIt.Value().Unload(InfoLoadManager, CommonSoundIDToInt(InfoMapIt.Key()), LoadedMainInfoPtrMap);
	//}
	//LoadedMainInfoPtrMap.Empty(); // 犬牢荤混
	//for (int32 CII = 0; CII < InfoArray_StageBGM.Num(); ++CII)
	//{
	//	InfoArray_StageBGM[CII].Unload(InfoLoadManager, CII, LoadedStageBGMPtrMap);
	//}
	//LoadedStageBGMPtrMap.Empty(); // 犬牢荤混

	//for (TMap<EUIScene, FSingleCommonSoundInfoMap_UIScene>::TIterator InfoMapIt(UISceneMap); InfoMapIt; ++InfoMapIt)
	//{
	//	InfoMapIt.Value().Unload(InfoLoadManager, static_cast<int32>(InfoMapIt.Key()), LoadedUISceneBGMPtrMap);
	//}
	//LoadedUISceneBGMPtrMap.Empty();

	//StageMiddleBGM.UnloadAll(InfoLoadManager, LoadedMiddleStageBGMPtrMap);
	//LoadedMiddleStageBGMPtrMap.Empty();
	//// ButtonClickDefSoundArray 绰 措惑 酒丛.
}

#if WITH_EDITOR
void UB2CommonSoundInfo::EditorLoadAll()
{
	if (GIsEditor == false)
	{
		return;
	}

	// 茄锅 固府 阂矾林搁 AssetPtr 捞 valid 秦 龙 巴. 拱沸 捞 客吝俊 皋葛府绰 空芒 冈摆瘤.
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

	// ButtonClickDefSoundArray 绰 措惑 酒丛.
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
		//FString WarnMsg = TEXT("促澜 StageBGM 亲格狼 RelevantStageKeyStr 捞 呈公 腹篮 胶抛捞瘤甫 器窃窍绊 乐嚼聪促. 捞绰 泪犁利牢 己瓷 历窍 夸家啊 登骨肺 淬寸 橇肺弊贰赣俊霸 利例茄 汲沥俊 措秦 巩狼窍技夸.\r\n\r\n");
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
		// 阿 FSingleCommonSoundInfoMap 夸家狼 SoundID 绰 殿废等 甘 虐蔼俊辑 啊廉咳.
		if (ThisInfoData.SoundID != ThisKey)
		{
			MarkPackageDirty();
		}
		ThisInfoData.SoundID = ThisKey;
	}
}
#endif

// TAsset lazy-loading 阑 荤侩窍绰 Blueprint InfoAsset 狼 坷悼累 巩力甫 沥侥栏肺 秦搬窍扁 傈鳖瘤狼 烙矫 东龙.
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

	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	CleanupOnPreSave();
}
#endif

USoundCue* FStageCustomizeBossBGM::GetSoundCue(FStreamableManager& InLoadManager, TMap<FName, FStageCustomizeBossBGMInfoLoadedRefHolder>& InLoadedStageBGMPtrMap, const FName& InName)
{
	FStageCustomizeBossBGMInfoLoadedRefHolder& SoundInfo = InLoadedStageBGMPtrMap.FindOrAdd(InName);
	USoundCue* TemplateCue = GetSomeLazyLoadedAssetCommon<USoundCue>(InLoadManager, SoundCuePtr, &(SoundInfo.LoadedSoundPtr), TEXT("CommonSoundInfo"));

	return TemplateCue;
}
