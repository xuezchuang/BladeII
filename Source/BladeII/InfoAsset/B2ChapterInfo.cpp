// Fill out your copyright notice in the Description page of Project Settings.


#include "B2ChapterInfo.h"
//#include "B2StageInfo.h"


#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2ChapterInfo::UB2ChapterInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
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
	{ // ChapterNum 弊措肺 key 肺 荤侩.
		FSingleChapterInfoData* FoundDataRow = TheData->FindRow<FSingleChapterInfoData>(FName(*FString::FromInt(ChapterNum)), TEXT(""));
		return FoundDataRow;
	}

	return NULL;
}

FSingleChapterInfoData* UB2ChapterInfo::GetChapterInfoOfStage(int32 InClientStageId)
{
	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	//// InClientStageId 俊 秦寸窍绰 StageInfoData 俊辑 Chapter 锅龋甫 啊廉客辑 荤侩.
	//FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(InClientStageId) : NULL;
	return /*StageInfoData ? GetInfoData(StageInfoData->ChapterNum) :*/ NULL;
}

int32 UB2ChapterInfo::GetAllChapterNum()
{
	/*if (ShouldShowCBTVersionContent())
		return 3;									//CBT滚傈 茅磐3栏肺 函版 抛捞喉 扒靛府绰芭焊促 亮促绊 积阿秦 惑荐肺 钦聪促*/

	return TheData->GetRowNames().Num();
}

int32 UB2ChapterInfo::GetStagesNumInChapter(int32 ChapterNum)
{
	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	//if (StageInfoTable)
	//{
	//	TArray<FSingleStageInfoData*> AllFoundStageInfo;
	//	StageInfoTable->GetAllInfoDataOfChapter(AllFoundStageInfo, ChapterNum);
	//	return AllFoundStageInfo.Num();
	//}
	return 0;
}

FText UB2ChapterInfo::GetLocalizedChapterName(int32 ChapterNum)
{
	FSingleChapterInfoData* ChapterInfoData = GetInfoData(ChapterNum);
	// WorldNameTextTable 阑 呼妨具摆促. 措脚 备盒捞 啊档废 prefix 甫 持绢焊绰 巴档 亮阑 淀.
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
				FString::Printf(TEXT("[版绊] ChapterInfo 单捞磐狼 临锅龋俊 摹疙利牢 巩力啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ChapterInfo 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
			));
#endif
	}
}
#endif