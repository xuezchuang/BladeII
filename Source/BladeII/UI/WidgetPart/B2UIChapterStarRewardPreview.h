// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2UIChapterStarRewardPreview.generated.h"

/*
 * Tiny preview right above the star reward box invoked when clicked reward box of non-reached state.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChapterStarRewardPreview : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:

	uint32 bCounterDungeonReward : 1;
	uint32 bCanGetReward : 1; // False means it is just preview only due to not enough gathered star.
	int32 ChapterNum;
	EStageDifficulty Difficulty;

	int32 MyRewardIndex; // Reward index among all other star reward preview boxes. Should exist reward box having the same RewardIndex.
	int32 CounterDungeonClear;

protected:

	/** Each reward icon will be dynamically created using this template. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIRewardIcon> RewardTemplate;
	/** Created based on actual reward items using RewardTemplate */
	TArray<class UB2UIRewardIcon*> CreatedRewardIcons;

	// Those materials are to be set to created RewardIcons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* RewardIcon_Gold;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* RewardIcon_Gem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 Reward_GoldIdx;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 Reward_GemIdx;

	TWeakObjectPtr<UHorizontalBox> X_Rewards; // Where the reward icons are to be created.

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_RewardStarCount;	
	TWeakObjectPtr<UTextBlock> TB_GetReward;

	TWeakObjectPtr<UB2Button> BTN_GetReward;
	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_Goods;

public:
	UB2UIChapterStarRewardPreview(const FObjectInitializer& ObjectInitializer);
		
	FORCEINLINE int32 GetRewardIndex() { return MyRewardIndex; }

	virtual void Init() override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
public:
	// Show until close button clicked. Passing most information through here.
	void ShowMe(bool bInCanGetReward, int32 InChapterNum, EStageDifficulty Difficulty, int32 InRewardIndex, int32 InMyCurrStarCount, int32 InMyRequiredStarCount, TArray<b2network::B2RewardPtr> InRewards);
	void JoinShowMe(bool bInCanGetReward, int32 InRewardIndex, int32 InMyJoinCount, int32 InMyRequiredJoinCount);

	void CloseMe();
	bool IsOpen();
	void CreateRewardIconCommon(class UMaterialInterface* InIconMat, const FText& InIconText);
	void CreateRewardIconCommon(ERewardType _Type, const FText& _InIconText);
	void CreateRewardIconCommon(int32 _rewardType, const FText& _InIconText);
	void CreateRewardIconCommon(b2network::B2RewardPtr InRewardItem, const FText& _InIconText);

	void SetRewardTemplateClass(TSubclassOf<class UB2UIRewardIcon> InNewTemplate);

	// Mainly to play opening animation.
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPopupBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void IsOnStar(bool bIsStarIcon);
	void SetPopupVisible(bool IsVisible);

private:

	void CreateRewardIcons(int32 InGoldAmount, int32 InGemAmount);
	void DestroyRewardIcons();

	//=================================================== Btn
	UFUNCTION()
	void OnClickBtnGetReward();
	UFUNCTION()
	void OnClickBtnClose();

};
