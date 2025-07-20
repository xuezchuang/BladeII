// Fill out your copyright notice in the Description page of Project Settings.


#include "B2StageInfo.h"
//#include "Engine/DataTable.h"
//#include "MoviePlayer.h"

#include "FB2ErrorMessage.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
//#include "BladeIIUtil.h"
#include "Misc/ConfigCacheIni.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"

UB2StageInfo::UB2StageInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FinalClientStageId = 0;
	StartAdventureModeChapterNum = 0;
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
		FString StageInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2StageManager"), TEXT("StageInfoDataTable"), StageInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *StageInfoDataTablePath);

		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("FinalStageNum"), FinalClientStageId, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("StartAdventureModeChapterNum"), StartAdventureModeChapterNum, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("EndAdventureModChapterNum"), EndAdventureModeChapterNum, GGameIni);

		SetDataByCBTVersionContent(); // 辆惫俊绰 力芭登绢具 且 淀.

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

void UB2StageInfo::SetDataByCBTVersionContent() // 捞吧 积己磊俊辑父 窍搁 摹飘肺 CBTVersionContent 咯何甫 官曹 锭 利侩捞 救凳.
{
	//if (ShouldShowCBTVersionContent()) //Gameini流立 扒靛府扁 弊贰辑 CBT滚傈老 版快 咯扁 碍力 技泼秦霖促
	//{
	//	FinalClientStageId = 30;
	//	StartAdventureModeChapterNum = 3;
	//}
	//else
	{
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("FinalStageNum"), FinalClientStageId, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("StartAdventureModeChapterNum"), StartAdventureModeChapterNum, GGameIni);
	}
}

FSingleStageInfoData* UB2StageInfo::GetInfoData(int32 InClientStageId)
{
	if (TheData)
	{
		// StageNum 弊措肺 key 肺 荤侩
		FSingleStageInfoData* FoundDataRow = TheData->FindRow<FSingleStageInfoData>(FName(*FString::FromInt(InClientStageId)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

FSingleStageInfoData* UB2StageInfo::GetInfoData(int32 InClientStageId, EStageDifficulty InDifficulty)
{
	int32 ClientStageID = ChangeDiffcultyStageID(InClientStageId, InDifficulty);

	if (TheData)
	{
		// StageNum 弊措肺 key 肺 荤侩
		FSingleStageInfoData* FoundDataRow = TheData->FindRow<FSingleStageInfoData>(FName(*FString::FromInt(ClientStageID)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

int32 UB2StageInfo::GetClientStageIdOfChapterNums(int32 InChapterNum, int32 InSubChapterNum)
{
	TArray<FName> RowKeys = TheData->GetRowNames();
	for (FName& ThisRowKey : RowKeys) // GetInfoData 贸烦 官肺 掺笼绢郴绰 霸 酒聪扼 iteration 阑 窍促焊聪 粱 蠢副 荐 乐摆促.
	{
		FSingleStageInfoData* FoundDataRow = TheData->FindRow<FSingleStageInfoData>(ThisRowKey, TEXT(""));
		if (FoundDataRow && FoundDataRow->ChapterNum == InChapterNum && FoundDataRow->SubChapterNum == InSubChapterNum)
		{
			return FCString::Atoi(*(ThisRowKey.ToString()));
		}
	}

	return 0;
}

int32 UB2StageInfo::GetChapterNumByClientStageId(int32 InClientStageId)
{
	if (TheData)
	{
		auto* RowData = reinterpret_cast<FSingleStageInfoData*>(TheData->GetRowMap().FindRef(FName(*FString::FromInt(InClientStageId))));
		if (RowData)
		{
			return RowData->ChapterNum;
		}
	}

	return 0;
}

int32 UB2StageInfo::GetSubChapterNumByClientStageId(int32 InClientStageId)
{
	if (TheData)
	{
		auto* RowData = reinterpret_cast<FSingleStageInfoData*>(TheData->GetRowMap().FindRef(FName(*FString::FromInt(InClientStageId))));
		if (RowData)
		{
			return RowData->SubChapterNum;
		}
	}

	return 0;
}

FSingleStageInfoData* UB2StageInfo::GetInfoDataByChapterNums(int32 InChapterNum, int32 InSubChapterNum)
{
	int32 FoundStageId = GetClientStageIdOfChapterNums(InChapterNum, InSubChapterNum);
	if (FoundStageId > 0)
		return GetInfoData(FoundStageId);

	return NULL;
}

void UB2StageInfo::GetAllInfoDataOfChapter(TArray<FSingleStageInfoData*>& OutFoundData, int32 InChapterToSearch)
{
	TArray<FName> RowKeys = TheData->GetRowNames();

	for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
	{
		FSingleStageInfoData* ThisInfoData = GetInfoData(FCString::Atoi(*RowKeys[RI].ToString()));
		if (ThisInfoData && ThisInfoData->ChapterNum == InChapterToSearch)
		{
			OutFoundData.Add(ThisInfoData);
		}
	}
}

int32 UB2StageInfo::GetMaxClientStageId()
{
	return TheData->GetRowNames().Num();
}


int32 UB2StageInfo::ChangeDiffcultyStageID(int32 InClientSTageID, EStageDifficulty InDifficulty)
{
	return InClientSTageID + ((static_cast<int32>(InDifficulty) - 1) * 200);
}

bool UB2StageInfo::IsInvokedGameMode(int32 InClientStageId, EB2GameMode InCheckGameMode)
{
	FSingleStageInfoData* StageInfoData = GetInfoData(InClientStageId);

	if (StageInfoData == nullptr)
		return false;

	return (StageInfoData->InvokedGameMode == InCheckGameMode);
}

#if WITH_EDITOR
void UB2StageInfo::CheckInfoDataIntegrity()
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

			if (RowKeyNumber <= 0)
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
				FString::Printf(TEXT("[版绊] StageInfo 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
		}

		// ChapterNum 苞 SubChapterNum 吝汗 救登档废 八荤
		int32 FoundDuplicatedChapterNumCombinationNum = 0;
		for (int32 RIA = 0; RIA < RowKeys.Num(); ++RIA)
		{
			FSingleStageInfoData* RowA = TheData->FindRow<FSingleStageInfoData>(RowKeys[RIA], TEXT(""));
			for (int32 RIB = RIA + 1; RIB < RowKeys.Num(); ++RIB)
			{
				FSingleStageInfoData* RowB = TheData->FindRow<FSingleStageInfoData>(RowKeys[RIB], TEXT(""));
				if (RowA && RowB && RowA->ChapterNum == RowB->ChapterNum && RowA->SubChapterNum == RowB->SubChapterNum)
				{
					++FoundDuplicatedChapterNumCombinationNum;
				}
			}
		}

		if (FoundDuplicatedChapterNumCombinationNum > 0)
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[版绊] StageInfo 狼 Chapter/SubChapterNum 汲沥 吝汗捞 %d 俺 惯斑登菌澜. 哪腔磐啊 梆 气惯茄促."), FoundDuplicatedChapterNumCombinationNum)
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("StageInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
		));
#endif
	}
}

void UB2StageInfo::EditorLoadAll()
{
	// DataTable 捞扼 redirector 贸府甫 犬角洒 窍扁 困秦 俊叼磐俊辑 TAsset 狼 full load 啊 鞘夸.
	EditorAllLoadedTAssetHolder.Empty();

	TArray<FName> RowKeys = TheData->GetRowNames();

	for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
	{
		FSingleStageInfoData* ThisInfoData = GetInfoData(FCString::Atoi(*RowKeys[RI].ToString()));
		if (ThisInfoData)
		{
			FB2StageInfoLoadedRefHolder SingleRefHolder;
			SingleRefHolder.LoadedStageTexture = ThisInfoData->GetStageTexture();
			SingleRefHolder.LoadedNPCTexture = ThisInfoData->GetNPCTexture();
			if (SingleRefHolder.LoadedStageTexture && SingleRefHolder.LoadedNPCTexture)
			{ // 弊成 持扁父 茄促. GC 救登档废.
				EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
			}


		}
	}
}
#endif

void FB2StageStreamingSettings::ExecuteStreaming(UWorld* InWorld,
	// LoadLimitTime 汲沥捞 乐阑 矫 秦寸 矫埃 捞饶 龋免瞪 Flush 鸥捞赣 沥焊甫 持绢林绢具 茄促. 角惑 LoadLimitTime 汲沥阑 救 敬促 秦档 持绢林绢具 窍绰 扒 函窃 绝澜 せ
	FTimerHandle& InTimerHandle,
	FTimerDelegate const& InFlushStreamingTimerDelegate)
{
	//B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FB2StageStreamingSettings::ExecuteStreaming %d Unload, %d Load, LoadLimitTime %.2f"), LevelToUnload.Num(), LevelToLoad.Num(), LoadLimitTime));

	// 扁盒惑捞变 且 搏单 Unload 何磐 窍磊 せ
	int32 StreamedCount = 0;
	for (const FName& ThisToUnload : LevelToUnload)
	{
		++StreamedCount;
		UGameplayStatics::UnloadStreamLevel(InWorld, ThisToUnload, FLatentActionInfo(),false);
	}

	// ShouldBlockOnLoad 矫 FlushAsyncLoading 捞 瞪 巴.. 奴 饭骇老荐废 捞巴 磊眉狼 肺爹 发档 努 波绊. 漂洒 胶抛捞瘤 矫累 檬馆俊绰 归弊扼款靛 async 肺爹捞 粱 乐绢辑 捞霸 甸绢啊搁 帮鄂且 巴.
	const bool bShouldBlockOnLoad = (LoadLimitTime <= 0.0f);

	TArray<ULevelStreaming*> AllLSObjToLoad;
	for (const FName& ThisToLoad : LevelToLoad)
	{
		++StreamedCount;
		UGameplayStatics::LoadStreamLevel(InWorld, ThisToLoad, true, bShouldBlockOnLoad, FLatentActionInfo());
	}
	if (StreamedCount > 0)
	{
		if (!bShouldBlockOnLoad && InWorld)
		{
			// 瘤沥茄 妮归俊辑 UGameplayStatics::FlushLevelStreaming 捞尔 TimerHandle 努府绢甫 秦具 茄促. 舅酒辑 肋.. ;;
			InWorld->GetTimerManager().SetTimer(InTimerHandle, InFlushStreamingTimerDelegate, LoadLimitTime, false);
		}
	}
}

#if WITH_EDITOR
void FB2StageStreamingSettings::EditorCheckForProperSetting(class UWorld* InWorld)
{
	TArray<FString> NotFoundNames;
	TArray<FString> WrongStreamingMethodNames;
	for (const FName& ThisToUnload : LevelToUnload)
	{
		// NAME_None 捞搁 酒流 技泼 救等 吧肺 摹磊.
		if (ThisToUnload != NAME_None && !EditorCheckLevelNameInWorld(InWorld, ThisToUnload))
		{
			NotFoundNames.Add(ThisToUnload.ToString());
		}
	}
	for (const FName& ThisToLoad : LevelToLoad)
	{
		const bool bExistNameCheck = EditorCheckLevelNameInWorld(InWorld, ThisToLoad);
		// NAME_None 捞搁 酒流 技泼 救等 吧肺 摹磊.
		if (ThisToLoad != NAME_None && !bExistNameCheck)
		{
			NotFoundNames.Add(ThisToLoad.ToString());
		}
		if (bExistNameCheck && !EditorCheckLevelStreamingMethodInWorld(InWorld, ThisToLoad))
		{ // 档吝俊 肺爹且 饭骇捞搁 AlwaysLoad 咯辑绰 救登瘤.
			WrongStreamingMethodNames.Add(ThisToLoad.ToString());
		}
	}

	if (NotFoundNames.Num() > 0)
	{
#if !PLATFORM_MAC
		FString NotFoundNameWarnMessage = TEXT("促澜 捞抚阑 泅犁 楷搬等 胶飘府怪 饭骇俊辑 茫阑 荐 绝嚼聪促.\n\n");
		for (const FString& ThisWrongLevel : NotFoundNames)
		{
			NotFoundNameWarnMessage += (ThisWrongLevel + TEXT("\n"));
		}
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(NotFoundNameWarnMessage));
#endif
	}
	if (WrongStreamingMethodNames.Num() > 0)
	{
#if !PLATFORM_MAC
		FString WrongStreamingWarnMessage = TEXT("促澜 捞抚篮 泅犁 楷搬等 胶飘府怪 饭骇俊辑 茫疽栏唱 Streaming Method 啊 肋给登菌嚼聪促.\n\n");
		for (const FString& ThisWrongLevel : WrongStreamingMethodNames)
		{
			WrongStreamingWarnMessage += (ThisWrongLevel + TEXT("\n"));
		}
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WrongStreamingWarnMessage));
#endif
	}
}

// Return true if InSL has InNameToCheck, considering PIE prefix.
bool CheckForStreamingLevelName(ULevelStreaming* InSL, const FString& InNameToCheck)
{
	if (InSL)
	{
		FString SLShortName = FPackageName::GetShortName(InSL->GetWorldAssetPackageName());

		// Play-in-Editor 侩 捞抚 立滴绢绰 哗绊 厚背.. 窍妨绊 沁歹聪 购 箭磊鳖瘤 甸绢啊辑 救登摆促.. 弊繁 版快绰 缔何盒父 厚背.
		if (
			(SLShortName.StartsWith(PLAYWORLD_PACKAGE_PREFIX) && SLShortName.EndsWith(InNameToCheck)) ||
			(InNameToCheck == SLShortName)
			)
		{
			return true;
		}
	}
	return false;
}

bool FB2StageStreamingSettings::EditorCheckLevelNameInWorld(class UWorld* InWorld, const FName& NameToCheck)
{
	if (InWorld)
	{
		const FString& NameToCheckStr = NameToCheck.ToString();

		for (ULevelStreaming* ThisSL : InWorld->GetStreamingLevels())
		{
			if (CheckForStreamingLevelName(ThisSL, NameToCheckStr))
			{
				return true;
			}
		}
	}
	return false;
}
bool FB2StageStreamingSettings::EditorCheckLevelStreamingMethodInWorld(class UWorld* InWorld, const FName& NameToCheck)
{
	if (InWorld)
	{
		const FString& NameToCheckStr = NameToCheck.ToString();

		for (ULevelStreaming* ThisSL : InWorld->GetStreamingLevels())
		{
			ULevelStreamingDynamic* ThisSLKismet = Cast<ULevelStreamingDynamic>(ThisSL);
			// 荤角惑 EditorCheckLevelNameInWorld 档 器窃
			if (ThisSLKismet && CheckForStreamingLevelName(ThisSLKismet, NameToCheckStr))
			{
				return true;
			}
		}
	}
	return false;
}
#endif


float FSingleStageInfoData::GetDifficultyLevel_ClientData(EStageDifficulty StageDifficulty)
{
	switch (StageDifficulty)
	{
	case EStageDifficulty::ESD_Normal:
		return DifficultyLevel;
	case EStageDifficulty::ESD_Hard:
		return DifficultyLevel_2;
	case EStageDifficulty::ESD_VeryHard:
		return DifficultyLevel_3;
	case EStageDifficulty::ESD_Hell:
		return DifficultyLevel_4;
	default:
		break;
	}

	return 1.f;
}

FText FSingleStageInfoData::GetLocalizedStageName()
{
	//return BladeIIGetLOCText(B2LOC_CAT_STAGENAME, LocalizationStageNameKey.ToString());
	return FText::GetEmpty();
}
FText FSingleStageInfoData::GetLocalizedStageStory()
{ // 悼老茄 虐蔼俊 category 父 促福促.
	//return BladeIIGetLOCText(B2LOC_CAT_STAGESTORY, LocalizationStageNameKey.ToString());
	return FText::GetEmpty();
}

/* ===========================================================================
 * 咯扁俊 乐绰 GMP 悼康惑 荤款靛 促惫绢 眉农 扁瓷甸篮 澄府 鞘夸秦瘤搁 焊促 惑困俊 傍烹 扁瓷栏肺 捞傈.
 * Stage 矫唱府坷 悼康惑父 捞繁 促惫绢 贸府啊 鞘夸茄 惑炔俊辑 咯扁俊 备泅茄 巴.
 * =========================================================================== */
FORCEINLINE FString CombineLocalizedMovieNameCommon(const FString& InRawMovieName, const FString& InLocNaming)
{
	return (InLocNaming.Len() > 0) ? FString::Printf(TEXT("%s_%s"), *InRawMovieName, *InLocNaming) : InRawMovieName;
}
bool DoesLocalizedMovieFileExist(const FString& InRawMovieName, int32 CheckSoundLocType)
{
	//IGameMoviePlayer* GMPObj = GetMoviePlayer();
	//if (GMPObj)
	//{
	//	const FString& CheckLocTypeMovieName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(CheckSoundLocType));
	//	// FPaths 率 窃荐 措脚 粱 芭芒茄 扁瓷栏肺 颇老 乐绰瘤甫 眉农窍绰单 角力 困摹窍绰 版肺啊 敲阀汽捞唱 汲摹 鸥涝俊 蝶扼 崔扼瘤扁 锭巩.
	//	if (GMPObj->DoesMovieFileExistAutoDLCCheck(CheckLocTypeMovieName))
	//	{
	//		return true;
	//	}
	//}
	return false;
}
const FString ConvertToSafeLocalizedMovieName(const FString& InRawMovieName)
{
	// 角力 秦寸 颇老 粮犁 咯何鳖瘤 蝶廉辑 府畔.
	// 促弗 镑俊辑档 荤侩 鞘夸啊 乐澜. 
	if (InRawMovieName.Len() > 0)
	{
		//{ // 泅犁 促惫绢 汲沥俊 弊措肺 概莫窍绰 捞抚阑 啊柳 颇老阑 刚历 茫澜.
		//	const int32 FirstTryType = FGenericPlatformMisc::GetSoundLocType();
		//	if (DoesLocalizedMovieFileExist(InRawMovieName, FirstTryType))
		//	{
		//		const FString& ResultMovieFileName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(FirstTryType));
		//		UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by current LOC type %d"), *ResultMovieFileName, FirstTryType);
		//		return ResultMovieFileName;
		//	}
		//}
		//{ // 泅犁 LocType 俊 概莫窍绰 捞抚阑 啊柳 颇老捞 绝阑 版快 促弗 芭扼档 茫酒夯促.  
		//	const int32 AltTryType = (FGenericPlatformMisc::GetSoundLocType() == B2SoundLocalizeType::English()) ?
		//		B2SoundLocalizeType::Korea() : B2SoundLocalizeType::English(); // 汲飞 捞巴 寇俊 促弗 促惫绢啊 乐歹扼档.. 2瞒 矫档绰 茄惫绢 酒丛 康绢啊 瞪 淀.
		//	if (DoesLocalizedMovieFileExist(InRawMovieName, AltTryType))
		//	{
		//		const FString& ResultMovieFileName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(AltTryType));
		//		//UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by Alt LOC type %d "), *ResultMovieFileName, AltTryType);
		//		return ResultMovieFileName;
		//	}
		//}
	}
	// 捞档历档 绝栏搁 suffix 救 嘿篮 raw 捞抚栏肺. 角力肺 促惫绢 贸府啊 鞘夸绝绰 康惑篮 捞犯霸 瞪 荐 乐澜.
	//UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by non-locaized raw name."), *InRawMovieName);
	return InRawMovieName;
}

const FString FSingleStageInfoData::GetLocalizedMovieName() const
{
	return ConvertToSafeLocalizedMovieName(MovieName);
}
bool FSingleStageInfoData::DoesCurrLocTypeMovieFileExist() const
{
	//return DoesLocalizedMovieFileExist(MovieName, FGenericPlatformMisc::GetSoundLocType());
	return true;
}
bool FSingleStageInfoData::HasScenarioMovie() const
{ // 角力 颇老 粮犁 蜡公焊促绰 汲沥捞 弊犯霸 登绢 乐绰瘤 眉农 狼固. 蝶扼辑 LocalizedMovieName 鳖瘤 杭 波 绝捞 MovieName 父 眉农.
	return (MovieName.Len() > 0);
}

UTexture2D* FSingleStageInfoData::GetStageTexture()
{
	return StageTexture.LoadSynchronous();
}

UTexture2D* FSingleStageInfoData::GetNPCTexture()
{
	return NPCTexture.LoadSynchronous();
}
