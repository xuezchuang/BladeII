// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIChapterStarRewardBox.generated.h"


/* This is when you click on informing purpose. */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnChapterStarRewardBoxClickedDelgate, UB2UIChapterStarRewardBox*, ClickedRewardBox);
DECLARE_DYNAMIC_DELEGATE(FRewardListDelgate);

/*
* This is the reward  a certain number of star gathered.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChapterStarRewardBox : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIChapterStarRewardBox(const FObjectInitializer& ObjectInitializer);
	
	void SetFillImage(EStageDifficulty InStageDifficulty);

	/* The settings that have already received rewards. */
	void SetRewarded(bool InAlreadyRewarded);
	void SetMyPreviousRewarded(bool InPreviousRewarded);
	void SetRewardIndex(int32 InIndex);

	/* The number of stars that gather to receive rewards. */
	void SetStarMaxNum(int32 InValue);
	void SetJoinMaxNum(int32 inValue);
	int32 GetStarMaxNum() const { return StarRewardMaxNum; }
	void SetStarCurNum(int32 InValue);

	/* When used force to put a different progress text. */
	void SetForceProgressText(int32 InCurValue, int32 InMaxValue);
	void SetRemainTime(int64 InCurValue, int64 InMaxValue);
	void SetForceProgressBar(int32 InCurValue, int32 InMaxValue);

	/* It is set that the rewards which is associated to a chapter. */
	void SetParentChapterNum(int32 Invalue);
	int32 GetParentChapterNum() const {return ParentChapterNum;}

	FORCEINLINE EUIChapterStarRewardState GetGoalReachedState() { return GoalReachedState; }
	FORCEINLINE int32 GetRewardIndex() { return MyRewardIndex; }

	void SetMissionBox(int32 CurrentPoint, int32 RequirePoint, bool bRewarded);
	void SetVisibleProgressText(bool bShow);

public:
	//Fulfilled the number of stars or not, or already rewarded.
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStarRewardBox")
	void SetGoalReachedState(EUIChapterStarRewardState RewardState);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStarRewardBox")
	void ShowProgressText(bool bShow);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStarRewardBox")
	void ShowStarText(bool bShow);
	
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void UpdateRewardState();

	//=================================================== Btn
	UFUNCTION()
		void OnClickBtnArea();

	//==================================================== Doc Delegate
public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TMap<EStageDifficulty, FSlateBrush> FillImages;

public:
	FOnChapterStarRewardBoxClickedDelgate	OnChapterStarRewardBoxClickedDelgate;
	FRewardListDelgate						RewardListDelgate;

protected:
	TWeakObjectPtr<UButton>					BTN_Area;
	TWeakObjectPtr<UTextBlock>				TB_StarCur;
	TWeakObjectPtr<UTextBlock>				TB_StarMax;
	TWeakObjectPtr<UTextBlock>				TB_StarGoalNum;
	TWeakObjectPtr<UProgressBar>			PB_Star;
	TWeakObjectPtr<class UB2RichTextBlock>	RTB_JoinGoalNum;
	TWeakObjectPtr<UHorizontalBox>			H_PBTexts;
	TWeakObjectPtr<UHorizontalBox>			HB_Star;
	TWeakObjectPtr<UHorizontalBox>			HB_MissionPoint;
	TWeakObjectPtr<UTextBlock>				TB_MissionGoalNum;

private:

	EUIChapterStarRewardState GoalReachedState;
	
	int32 MyRewardIndex; // Reward index among all other star reward boxes. It's also the same to server's reward_index.

	int32 StarRewardMaxNum;
	int32 StarRewardCurNum;
	bool bMyPreviousRewarded; // I can get reward once after received my previous reward.
	bool bAlreadyRewarded;
	
	int32 ParentChapterNum;
};
