#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIModWeekRewardPopup.generated.h"

/*
* Lobby Main UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIModWeekRewardPopup : public UB2UIWidget
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<class UB2UIGradeRankAndStar>		UIP_LargeGrade;
	TWeakObjectPtr<UTextBlock>						TB_MyRank;
	TWeakObjectPtr<UTextBlock>						TB_RankPercent;
	TWeakObjectPtr<UTextBlock>						TB_RewardGem;
	TWeakObjectPtr<UButton>							BTN_Close;
	


	//StaticText
	TWeakObjectPtr<UTextBlock>						TB_WeekRewardTitle;
	TWeakObjectPtr<UTextBlock>						TB_MyRanking;
	TWeakObjectPtr<UTextBlock>						TB_MyRankingUnit;
	TWeakObjectPtr<UTextBlock>						TB_RewardExplanation1;
	TWeakObjectPtr<UTextBlock>						TB_RewardExplanation2;
	TWeakObjectPtr<UTextBlock>						TB_SubTitle;
	TWeakObjectPtr<UTextBlock>						TB_BestGrade;
	TWeakObjectPtr<UTextBlock>						TB_WinNumber;
	TWeakObjectPtr<UTextBlock>						TB_MaximumWin;

	TArray<TWeakObjectPtr<UImage>>					RewardItemImage;
	TArray<TWeakObjectPtr<UTextBlock>>				RewardItemCount;
	TArray<TWeakObjectPtr<UPanelWidget>>			RewardItemPanel;

	static const int32 MAX_REWARD_ITEM = 3;

public:
	UFUNCTION()
		void OnClickClose();

	void OpenReward(FName UIType);

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	void OpenSetting(b2network::B2RankingRewardPtr Reward);
};
