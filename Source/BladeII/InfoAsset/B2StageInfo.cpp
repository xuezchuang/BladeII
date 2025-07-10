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
		// 정해진 하나를 로딩
		FString StageInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2StageManager"), TEXT("StageInfoDataTable"), StageInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *StageInfoDataTablePath);

		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("FinalStageNum"), FinalClientStageId, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("StartAdventureModeChapterNum"), StartAdventureModeChapterNum, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2StageManager"), TEXT("EndAdventureModChapterNum"), EndAdventureModeChapterNum, GGameIni);

		SetDataByCBTVersionContent(); // 종국에는 제거되어야 할 듯.

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

void UB2StageInfo::SetDataByCBTVersionContent() // 이걸 생성자에서만 하면 치트로 CBTVersionContent 여부를 바꿀 때 적용이 안됨.
{
	//if (ShouldShowCBTVersionContent()) //Gameini직접 건드리기 그래서 CBT버전일 경우 여기 강제 세팅해준다
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
		// StageNum 그대로 key 로 사용
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
		// StageNum 그대로 key 로 사용
		FSingleStageInfoData* FoundDataRow = TheData->FindRow<FSingleStageInfoData>(FName(*FString::FromInt(ClientStageID)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

int32 UB2StageInfo::GetClientStageIdOfChapterNums(int32 InChapterNum, int32 InSubChapterNum)
{
	TArray<FName> RowKeys = TheData->GetRowNames();
	for (FName& ThisRowKey : RowKeys) // GetInfoData 처럼 바로 끄집어내는 게 아니라 iteration 을 하다보니 좀 느릴 수 있겠다.
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

		// DataTable 의 모든 row 를 iterate 하는 방법인듯.
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber <= 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 이면 결국 여기도..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // 순서대로일 필요는 없을 것 같다. 일단 이건 경고에서 제거.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[경고] StageInfo 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
			));
#endif
		}

		// ChapterNum 과 SubChapterNum 중복 안되도록 검사
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
				FString::Printf(TEXT("[경고] StageInfo 의 Chapter/SubChapterNum 설정 중복이 %d 개 발견되었음. 컴퓨터가 곧 폭발한다."), FoundDuplicatedChapterNumCombinationNum)
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("StageInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
		));
#endif
	}
}

void UB2StageInfo::EditorLoadAll()
{
	// DataTable 이라 redirector 처리를 확실히 하기 위해 에디터에서 TAsset 의 full load 가 필요.
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
			{ // 그냥 넣기만 한다. GC 안되도록.
				EditorAllLoadedTAssetHolder.Add(SingleRefHolder);
			}


		}
	}
}
#endif

void FB2StageStreamingSettings::ExecuteStreaming(UWorld* InWorld,
	// LoadLimitTime 설정이 있을 시 해당 시간 이후 호출될 Flush 타이머 정보를 넣어주어야 한다. 실상 LoadLimitTime 설정을 안 쓴다 해도 넣어주어야 하는 건 변함 없음 ㅋ
	FTimerHandle& InTimerHandle,
	FTimerDelegate const& InFlushStreamingTimerDelegate)
{
	//B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FB2StageStreamingSettings::ExecuteStreaming %d Unload, %d Load, LoadLimitTime %.2f"), LevelToUnload.Num(), LevelToLoad.Num(), LoadLimitTime));

	// 기분상이긴 할 껀데 Unload 부터 하자 ㅋ
	int32 StreamedCount = 0;
	for (const FName& ThisToUnload : LevelToUnload)
	{
		++StreamedCount;
		UGameplayStatics::UnloadStreamLevel(InWorld, ThisToUnload, FLatentActionInfo(),false);
	}

	// ShouldBlockOnLoad 시 FlushAsyncLoading 이 될 것.. 큰 레벨일수록 이것 자체의 로딩 랙도 클 꺼고. 특히 스테이지 시작 초반에는 백그라운드 async 로딩이 좀 있어서 이게 들어가면 곤란할 것.
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
			// 지정한 콜백에서 UGameplayStatics::FlushLevelStreaming 이랑 TimerHandle 클리어를 해야 한다. 알아서 잘.. ;;
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
		// NAME_None 이면 아직 세팅 안된 걸로 치자.
		if (ThisToUnload != NAME_None && !EditorCheckLevelNameInWorld(InWorld, ThisToUnload))
		{
			NotFoundNames.Add(ThisToUnload.ToString());
		}
	}
	for (const FName& ThisToLoad : LevelToLoad)
	{
		const bool bExistNameCheck = EditorCheckLevelNameInWorld(InWorld, ThisToLoad);
		// NAME_None 이면 아직 세팅 안된 걸로 치자.
		if (ThisToLoad != NAME_None && !bExistNameCheck)
		{
			NotFoundNames.Add(ThisToLoad.ToString());
		}
		if (bExistNameCheck && !EditorCheckLevelStreamingMethodInWorld(InWorld, ThisToLoad))
		{ // 도중에 로딩할 레벨이면 AlwaysLoad 여서는 안되지.
			WrongStreamingMethodNames.Add(ThisToLoad.ToString());
		}
	}

	if (NotFoundNames.Num() > 0)
	{
#if !PLATFORM_MAC
		FString NotFoundNameWarnMessage = TEXT("다음 이름을 현재 연결된 스트리밍 레벨에서 찾을 수 없습니다.\n\n");
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
		FString WrongStreamingWarnMessage = TEXT("다음 이름은 현재 연결된 스트리밍 레벨에서 찾았으나 Streaming Method 가 잘못되었습니다.\n\n");
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

		// Play-in-Editor 용 이름 접두어는 빼고 비교.. 하려고 했더니 뭔 숫자까지 들어가서 안되겠다.. 그런 경우는 뒷부분만 비교.
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
			// 사실상 EditorCheckLevelNameInWorld 도 포함
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
{ // 동일한 키값에 category 만 다르다.
	//return BladeIIGetLOCText(B2LOC_CAT_STAGESTORY, LocalizationStageNameKey.ToString());
	return FText::GetEmpty();
}

/* ===========================================================================
 * 여기에 있는 GMP 동영상 사운드 다국어 체크 기능들은 널리 필요해지면 보다 상위에 공통 기능으로 이전.
 * Stage 시나리오 동영상만 이런 다국어 처리가 필요한 상황에서 여기에 구현한 것.
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
	//	// FPaths 쪽 함수 대신 좀 거창한 기능으로 파일 있는지를 체크하는데 실제 위치하는 경로가 플랫폼이나 설치 타입에 따라 달라지기 때문.
	//	if (GMPObj->DoesMovieFileExistAutoDLCCheck(CheckLocTypeMovieName))
	//	{
	//		return true;
	//	}
	//}
	return false;
}
const FString ConvertToSafeLocalizedMovieName(const FString& InRawMovieName)
{
	// 실제 해당 파일 존재 여부까지 따져서 리턴.
	// 다른 곳에서도 사용 필요가 있음. 
	if (InRawMovieName.Len() > 0)
	{
		//{ // 현재 다국어 설정에 그대로 매칭하는 이름을 가진 파일을 먼저 찾음.
		//	const int32 FirstTryType = FGenericPlatformMisc::GetSoundLocType();
		//	if (DoesLocalizedMovieFileExist(InRawMovieName, FirstTryType))
		//	{
		//		const FString& ResultMovieFileName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(FirstTryType));
		//		UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by current LOC type %d"), *ResultMovieFileName, FirstTryType);
		//		return ResultMovieFileName;
		//	}
		//}
		//{ // 현재 LocType 에 매칭하는 이름을 가진 파일이 없을 경우 다른 거라도 찾아본다.  
		//	const int32 AltTryType = (FGenericPlatformMisc::GetSoundLocType() == B2SoundLocalizeType::English()) ?
		//		B2SoundLocalizeType::Korea() : B2SoundLocalizeType::English(); // 설령 이것 외에 다른 다국어가 있더라도.. 2차 시도는 한국어 아님 영어가 될 듯.
		//	if (DoesLocalizedMovieFileExist(InRawMovieName, AltTryType))
		//	{
		//		const FString& ResultMovieFileName = CombineLocalizedMovieNameCommon(InRawMovieName, B2SoundLocalizeType::GetNamingConvention(AltTryType));
		//		//UE_LOG(LogBladeII, Log, TEXT("ConvertToSafeLocalizedMovieName %s, by Alt LOC type %d "), *ResultMovieFileName, AltTryType);
		//		return ResultMovieFileName;
		//	}
		//}
	}
	// 이도저도 없으면 suffix 안 붙은 raw 이름으로. 실제로 다국어 처리가 필요없는 영상은 이렇게 될 수 있음.
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
{ // 실제 파일 존재 유무보다는 설정이 그렇게 되어 있는지 체크 의미. 따라서 LocalizedMovieName 까지 볼 꺼 없이 MovieName 만 체크.
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
