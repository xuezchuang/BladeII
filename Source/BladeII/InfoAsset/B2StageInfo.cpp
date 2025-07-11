// Fill out your copyright notice in the Description page of Project Settings.


#include "B2StageInfo.h"
//#include "Engine/DataTable.h"
//#include "MoviePlayer.h"

#include "FB2ErrorMessage.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
//#include "BladeIIUtil.h"

UB2StageInfo::UB2StageInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FinalClientStageId = 0;
	StartAdventureModeChapterNum = 0;
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// ������ �ϳ��� �ε�
		FString StageInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2StageManager"), TEXT("StageInfoDataTable"), StageInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *StageInfoDataTablePath);

		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("FinalStageNum"), FinalClientStageId, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("StartAdventureModeChapterNum"), StartAdventureModeChapterNum, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("EndAdventureModChapterNum"), EndAdventureModeChapterNum, GGameIni);

		SetDataByCBTVersionContent(); // �������� ���ŵǾ�� �� ��.

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

void UB2StageInfo::SetDataByCBTVersionContent() // �̰� �����ڿ����� �ϸ� ġƮ�� CBTVersionContent ���θ� �ٲ� �� ������ �ȵ�.
{
	//if (ShouldShowCBTVersionContent()) //Gameini���� �ǵ帮�� �׷��� CBT������ ��� ���� ���� �������ش�
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
		// StageNum �״�� key �� ���
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
		// StageNum �״�� key �� ���
		FSingleStageInfoData* FoundDataRow = TheData->FindRow<FSingleStageInfoData>(FName(*FString::FromInt(ClientStageID)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

int32 UB2StageInfo::GetClientStageIdOfChapterNums(int32 InChapterNum, int32 InSubChapterNum)
{
	TArray<FName> RowKeys = TheData->GetRowNames();
	for (FName& ThisRowKey : RowKeys) // GetInfoData ó�� �ٷ� ������� �� �ƴ϶� iteration �� �ϴٺ��� �� ���� �� �ְڴ�.
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

		// DataTable �� ��� row �� iterate �ϴ� ����ε�.
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber <= 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 �̸� �ᱹ ���⵵..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // ��������� �ʿ�� ���� �� ����. �ϴ� �̰� ����� ����.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[���] StageInfo �������� �ٹ�ȣ�� ġ������ ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif
		}

		// ChapterNum �� SubChapterNum �ߺ� �ȵǵ��� �˻�
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
				FString::Printf(TEXT("[���] StageInfo �� Chapter/SubChapterNum ���� �ߺ��� %d �� �߰ߵǾ���. ��ǻ�Ͱ� �� �����Ѵ�."), FoundDuplicatedChapterNumCombinationNum)
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("StageInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
		));
#endif
	}
}

void UB2StageInfo::EditorLoadAll()
{
	// DataTable �̶� redirector ó���� Ȯ���� �ϱ� ���� �����Ϳ��� TAsset �� full load �� �ʿ�.
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
			{ // �׳� �ֱ⸸ �Ѵ�. GC �ȵǵ���.
				EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
			}


		}
	}
}
#endif

void FB2StageStreamingSettings::ExecuteStreaming(UWorld* InWorld,
	// LoadLimitTime ������ ���� �� �ش� �ð� ���� ȣ��� Flush Ÿ�̸� ������ �־��־�� �Ѵ�. �ǻ� LoadLimitTime ������ �� ���� �ص� �־��־�� �ϴ� �� ���� ���� ��
	FTimerHandle& InTimerHandle,
	FTimerDelegate const& InFlushStreamingTimerDelegate)
{
	//B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FB2StageStreamingSettings::ExecuteStreaming %d Unload, %d Load, LoadLimitTime %.2f"), LevelToUnload.Num(), LevelToLoad.Num(), LoadLimitTime));

	// ��л��̱� �� ���� Unload ���� ���� ��
	int32 StreamedCount = 0;
	for (const FName& ThisToUnload : LevelToUnload)
	{
		++StreamedCount;
		UGameplayStatics::UnloadStreamLevel(InWorld, ThisToUnload, FLatentActionInfo(),false);
	}

	// ShouldBlockOnLoad �� FlushAsyncLoading �� �� ��.. ū �����ϼ��� �̰� ��ü�� �ε� ���� Ŭ ����. Ư�� �������� ���� �ʹݿ��� ��׶��� async �ε��� �� �־ �̰� ���� ����� ��.
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
			// ������ �ݹ鿡�� UGameplayStatics::FlushLevelStreaming �̶� TimerHandle Ŭ��� �ؾ� �Ѵ�. �˾Ƽ� ��.. ;;
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
		// NAME_None �̸� ���� ���� �ȵ� �ɷ� ġ��.
		if (ThisToUnload != NAME_None && !EditorCheckLevelNameInWorld(InWorld, ThisToUnload))
		{
			NotFoundNames.Add(ThisToUnload.ToString());
		}
	}
	for (const FName& ThisToLoad : LevelToLoad)
	{
		const bool bExistNameCheck = EditorCheckLevelNameInWorld(InWorld, ThisToLoad);
		// NAME_None �̸� ���� ���� �ȵ� �ɷ� ġ��.
		if (ThisToLoad != NAME_None && !bExistNameCheck)
		{
			NotFoundNames.Add(ThisToLoad.ToString());
		}
		if (bExistNameCheck && !EditorCheckLevelStreamingMethodInWorld(InWorld, ThisToLoad))
		{ // ���߿� �ε��� �����̸� AlwaysLoad ������ �ȵ���.
			WrongStreamingMethodNames.Add(ThisToLoad.ToString());
		}
	}

	if (NotFoundNames.Num() > 0)
	{
#if !PLATFORM_MAC
		FString NotFoundNameWarnMessage = TEXT("���� �̸��� ���� ����� ��Ʈ���� �������� ã�� �� �����ϴ�.\n\n");
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
		FString WrongStreamingWarnMessage = TEXT("���� �̸��� ���� ����� ��Ʈ���� �������� ã������ Streaming Method �� �߸��Ǿ����ϴ�.\n\n");
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

		// Play-in-Editor �� �̸� ���ξ�� ���� ��.. �Ϸ��� �ߴ��� �� ���ڱ��� ���� �ȵǰڴ�.. �׷� ���� �޺κи� ��.
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
			// ��ǻ� EditorCheckLevelNameInWorld �� ����
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
{ // ������ Ű���� category �� �ٸ���.
	//return BladeIIGetLOCText(B2LOC_CAT_STAGESTORY, LocalizationStageNameKey.ToString());
	return FText::GetEmpty();
}

/* ===========================================================================
 * ���⿡ �ִ� GMP ������ ���� �ٱ��� üũ ��ɵ��� �θ� �ʿ������� ���� ������ ���� ������� ����.
 * Stage �ó����� ������ �̷� �ٱ��� ó���� �ʿ��� ��Ȳ���� ���⿡ ������ ��.
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
	//	// FPaths �� �Լ� ��� �� ��â�� ������� ���� �ִ����� üũ�ϴµ� ���� ��ġ�ϴ� ��ΰ� �÷����̳� ��ġ Ÿ�Կ� ���� �޶����� ����.
	//	if (GMPObj->DoesMovieFileExistAutoDLCCheck(CheckLocTypeMovieName))
	//	{
	//		return true;
	//	}
	//}
	return false;
}
const FString ConvertToSafeLocalizedMovieName(const FString& InRawMovieName)
{
	// ���� �ش� ���� ���� ���α��� ������ ����.
	// �ٸ� �������� ��� �ʿ䰡 ����. 
	if (InRawMovieName.Len() > 0)
	{
		//{ // ���� �ٱ��� ������ �״�� ��Ī�ϴ� �̸��� ���� ������ ���� ã��.
		//	const int32 FirstTryType = FGenericPlatformMisc::GetSoundLocType();
		//	if (DoesLocalizedMovieFileExist(InRawMovieName, FirstTryType))
		//	{
		//		const FString& ResultMovieFileName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(FirstTryType));
		//		UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by current LOC type %d"), *ResultMovieFileName, FirstTryType);
		//		return ResultMovieFileName;
		//	}
		//}
		//{ // ���� LocType �� ��Ī�ϴ� �̸��� ���� ������ ���� ��� �ٸ� �Ŷ� ã�ƺ���.  
		//	const int32 AltTryType = (FGenericPlatformMisc::GetSoundLocType() == B2SoundLocalizeType::English()) ?
		//		B2SoundLocalizeType::Korea() : B2SoundLocalizeType::English(); // ���� �̰� �ܿ� �ٸ� �ٱ�� �ִ���.. 2�� �õ��� �ѱ��� �ƴ� ��� �� ��.
		//	if (DoesLocalizedMovieFileExist(InRawMovieName, AltTryType))
		//	{
		//		const FString& ResultMovieFileName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(AltTryType));
		//		//UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by Alt LOC type %d "), *ResultMovieFileName, AltTryType);
		//		return ResultMovieFileName;
		//	}
		//}
	}
	// �̵����� ������ suffix �� ���� raw �̸�����. ������ �ٱ��� ó���� �ʿ���� ������ �̷��� �� �� ����.
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
{ // ���� ���� ���� �������ٴ� ������ �׷��� �Ǿ� �ִ��� üũ �ǹ�. ���� LocalizedMovieName ���� �� �� ���� MovieName �� üũ.
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
