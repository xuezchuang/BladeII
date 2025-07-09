// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIDimensionMain.generated.h"


UCLASS()
class BLADEII_API UB2UIDimensionMain : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SubscribeEvent();
	void UnsubscribeEvent();

private:
	void UpdateRanking();
	void UpdateMyRanking();

	void UpdateMyRankDetail();
	void UpdateDailyCount();

	void UpdateDiffReward();
	void UpdateCombatPower();
	void UpdateDiffPanel();

	void ShowWeekReward();

protected:
	UFUNCTION()
	void OnClickNext();
	UFUNCTION()
	void OnClickNextEnd();
	UFUNCTION()
	void OnClickPrev();
	UFUNCTION()
	void OnClickPrevEnd();
	UFUNCTION()
	void OnClickBtnStart();
	UFUNCTION()
	void OnClickBtnWeeklyReward();

private:
	TWeakObjectPtr<class UB2UIDimensionRankerInfo> UIP_MyRank;
	TWeakObjectPtr<UTextBlock> STB_RewardTitle;
	TWeakObjectPtr<UTextBlock> STB_StepTitle;
	TWeakObjectPtr<UTextBlock> STB_ClearTime;
	TWeakObjectPtr<UTextBlock> STB_MaxStep;
	TWeakObjectPtr<UTextBlock> STB_MyRank;
	TWeakObjectPtr<UTextBlock> STB_RankDesc;
	TWeakObjectPtr<UTextBlock> STB_DailyCount;
	TWeakObjectPtr<UTextBlock> STB_RankTitle;

	TWeakObjectPtr<UTextBlock> TB_MaxStep;
	TWeakObjectPtr<UTextBlock> TB_ClearTime;
	TWeakObjectPtr<UTextBlock> TB_DailyCount;
	TWeakObjectPtr<UTextBlock> TB_Diffulty;

	TWeakObjectPtr<class UB2UIChapterStarRewardBox> UIP_WeekRewardBox;
	TWeakObjectPtr<UHorizontalBox> List_Reward;
	TWeakObjectPtr<UVerticalBox> List_Ranker;
	TWeakObjectPtr<UB2Button> BTN_Next;
	TWeakObjectPtr<UB2Button> BTN_NextEnd;
	TWeakObjectPtr<UB2Button> BTN_Prev;
	TWeakObjectPtr<UB2Button> BTN_PrevEnd;
	TWeakObjectPtr<class UB2UIRecommandPower> UIP_RecommendPower;
	TWeakObjectPtr<class UB2UIStartButton> UIP_StartBT;

	TArray<std::shared_ptr<class issue>> Issues;
	int32 CurrentDiff;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIRewardIcon>		RewardSlot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIDimensionRankerInfo>		RankingSlot;
};

UCLASS()
class BLADEII_API UB2UIDimensionRankerInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void UpdateStaticText() override;
	void SetRankerInfo(const FModRankerInfo& RankerInfo);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	

public:
	UFUNCTION()
		void OnClickSelect();

private:
	TWeakObjectPtr<UTextBlock> TB_Ranking;
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_GradeIcon;
	TWeakObjectPtr<class UB2UIPVPEntryCharacters> UIP_EntryCharacters;
	TWeakObjectPtr<UButton> BTN_ShowPlayerInfo;

	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_PlayerName;
	TWeakObjectPtr<UTextBlock> TB_ClearTime;
	TWeakObjectPtr<UTextBlock> TB_ClearTower;

	TWeakObjectPtr<UImage> IMG_RankNumber_1;
	TWeakObjectPtr<UImage> IMG_RankNumber_2;
	TWeakObjectPtr<UImage> IMG_RankNumber_3;
	TWeakObjectPtr<UImage> IMG_RankBG_1;
	TWeakObjectPtr<UImage> IMG_RankBG_2;
	TWeakObjectPtr<UImage> IMG_RankBG_3;

	TWeakObjectPtr<UB2Button> BTN_Select;

private:
	int64			AccountInfo;
	EPCClass		SubClass;
	EPCClass		MainClass;
	FString			UserName;
};

