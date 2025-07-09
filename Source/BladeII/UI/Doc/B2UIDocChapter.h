// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "CommonStruct.h"
#include "B2UIDocChapter.generated.h"

/* To be recognized in the FUNCTION must be USTRUCT. */
USTRUCT()
struct FUIChapterStarReward
{
	GENERATED_USTRUCT_BODY()

	FUIChapterStarReward(){}
	FUIChapterStarReward(int32 InStarMaxNum, int32 InStarCurNum, bool InAlreadyRewarded)
	{
		StarMaxNum = InStarMaxNum;
		StarCurNum = InStarCurNum;
		bAlreadyRewarded = InAlreadyRewarded;
	}

public:
	int32				StarMaxNum;
	int32				StarCurNum;
	bool				bAlreadyRewarded;

};

/* This purpose is to hold miscellaneous UI-related data. */
UCLASS()
class BLADEII_API UB2UIDocChapter : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocChapter(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocChapter();

public:
	// =========== AdventureSelectMode
	DECLARE_UIDOC_FIELD(int32, NextChapterNum)
	DECLARE_UIDOC_FIELD(int32, NextSubChapterNum)
	DECLARE_UIDOC_FIELD(EStageDifficulty, MaxOpenedStageDifficulty)
	DECLARE_UIDOC_FIELD(EStageDifficulty, SelectHardStageDifficulty)

	// =========== Chapter
	DECLARE_UIDOC_FIELD(int32, ChapterOpenedNum)		//Last reached chapter number. "The 1 Chapter"
	DECLARE_UIDOC_FIELD(int32, ChapterMaxNum)			//Total number of chapters that exist.

	DECLARE_UIDOC_FIELD(int32, CurChapterNum)			//The number of the chapter you are currently viewing.
	DECLARE_UIDOC_FIELD(FText, CurChapterName)			//The name of the chapter you are currently viewing.

	DECLARE_UIDOC_FIELD(EStageDifficulty, CurStageDifficulty)		// The Difficulty of the AdventureMod you are currently viewing.
	DECLARE_UIDOC_FIELD(TArray<FUIChapterStarReward>, StarRewards) //The Array of Reward infos of CurChapter.

	DECLARE_UIDOC_FIELD(TArray<b2network::B2RewardPtr>, Rewards);

	DECLARE_UIDOC_FIELD(bool , ShowChapterEventScene)
	DECLARE_UIDOC_FIELD(FServerStageID, DeferredServerStageID)

	// Tutorial Check
	DECLARE_UIDOC_FIELD(bool, TutorialFixSwipe)

private:
};
