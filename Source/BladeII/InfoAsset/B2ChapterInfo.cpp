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
		// ������ �ϳ��� �ε�
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
	{ // ChapterNum �״�� key �� ���.
		FSingleChapterInfoData* FoundDataRow = TheData->FindRow<FSingleChapterInfoData>(FName(*FString::FromInt(ChapterNum)), TEXT(""));
		return FoundDataRow;
	}

	return NULL;
}

FSingleChapterInfoData* UB2ChapterInfo::GetChapterInfoOfStage(int32 InClientStageId)
{
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	// InClientStageId �� �ش��ϴ� StageInfoData ���� Chapter ��ȣ�� �����ͼ� ���.
	FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(InClientStageId) : NULL;
	return StageInfoData ? GetInfoData(StageInfoData->ChapterNum) : NULL;
}

int32 UB2ChapterInfo::GetAllChapterNum()
{
	/*if (ShouldShowCBTVersionContent())
		return 3;									//CBT���� é��3���� ���� ���̺� �ǵ帮�°ź��� ���ٰ� ������ ����� �մϴ�*/

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
	// WorldNameTextTable �� �����߰ڴ�. ��� ������ ������ prefix �� �־�� �͵� ���� ��.
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
				FString::Printf(TEXT("[���] ChapterInfo �������� �ٹ�ȣ�� ġ������ ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ChapterInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif
	}
}
#endif