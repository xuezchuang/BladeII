// Fill out your copyright notice in the Description page of Project Settings.


#include "B2ChapterInfo.h"
#include "B2StageInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2ChapterInfo::UB2ChapterInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
		FString ChapterInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2StageManager"), TEXT("ChapterInfoDataTable"), ChapterInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *ChapterInfoDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FSingleChapterInfoData* UB2ChapterInfo::GetInfoData(int32 ChapterNum)
{
	if (TheData)
	{ // ChapterNum 그대로 key 로 사용.
		FSingleChapterInfoData* FoundDataRow = TheData->FindRow<FSingleChapterInfoData>(FName(*FString::FromInt(ChapterNum)), TEXT(""));
		return FoundDataRow;
	}

	return NULL;
}

FSingleChapterInfoData* UB2ChapterInfo::GetChapterInfoOfStage(int32 InClientStageId)
{
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	// InClientStageId 에 해당하는 StageInfoData 에서 Chapter 번호를 가져와서 사용.
	FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(InClientStageId) : NULL;
	return StageInfoData ? GetInfoData(StageInfoData->ChapterNum) : NULL;
}

int32 UB2ChapterInfo::GetAllChapterNum()
{
	/*if (ShouldShowCBTVersionContent())
		return 3;									//CBT버전 챕터3으로 변경 테이블 건드리는거보다 좋다고 생각해 상수로 합니다*/

	return TheData->GetRowNames().Num();
}

int32 UB2ChapterInfo::GetStagesNumInChapter(int32 ChapterNum)
{
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	if (StageInfoTable)
	{
		TArray<FSingleStageInfoData*> AllFoundStageInfo;
		StageInfoTable->GetAllInfoDataOfChapter(AllFoundStageInfo, ChapterNum);
		return AllFoundStageInfo.Num();
	}
	return 0;
}

FText UB2ChapterInfo::GetLocalizedChapterName(int32 ChapterNum)
{
	FSingleChapterInfoData* ChapterInfoData = GetInfoData(ChapterNum);
	// WorldNameTextTable 을 빌려야겠다. 대신 구분이 가도록 prefix 를 넣어보는 것도 좋을 듯.
	return ChapterInfoData ? BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, ChapterInfoData->LocalizedChapterNameKey.ToString()) : FText::FromString(TEXT(""));
}

FText UB2ChapterInfo::GetLocalizedDifficultyName(EStageDifficulty difficulty)
{
	switch (difficulty)
	{
	case EStageDifficulty::ESD_Normal:
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_NormalTitle")));
	case EStageDifficulty::ESD_Hard:
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_HardTitle")));
	case EStageDifficulty::ESD_VeryHard :
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_VeryHardTitle")));
	case EStageDifficulty::ESD_Hell:
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AdventureMod_HellTitle")));
	}

	return FText();
}

#if WITH_EDITOR
void UB2ChapterInfo::CheckInfoDataIntegrity()
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
				FString::Printf(TEXT("[경고] ChapterInfo 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ChapterInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
			));
#endif
	}
}
#endif