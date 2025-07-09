// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIKakaoFriendRewardRow.generated.h"

enum class EFriendRewardUIState : uint8
{
	None,
	Progesss,
	GetReward,
	Complete,
	Max
};

class UB2UIFriendKakaoInvite;

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIKakaoFriendRewardRow : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	UB2UIKakaoFriendRewardRow(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	
	int32 GetRowIndex() const { return RowIndex; }
	void SetRowIndex(const int32 Index) { RowIndex = Index; }

	void SetRewardState(EFriendRewardUIState State);
	void SetRewardInfo(const b2network::B2mdKakaoFriendInviteRewardPtr &RewardInfo, bool IsInviteMode);
	void SetRewardState(const bool IsGetReward, const int32 CurCount, const int32 ConditionCount);
	void SetRewardInfoEveryJoin(const int32 RewardItemId, const int32 RwardItemAmount);
	void SetRewardInfoEveryJoinTip(FText Message);

	void SetOwnerUI(UB2UIFriendKakaoInvite *InOwnerUI) { OwnerUI = InOwnerUI; }
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickReward();

private:
	void SetProgressState();
	void SetGetRewardState();
	void SetCompleteState();
	void SetRewardJoinTipState();

private:
	int32 RowIndex;

	EFriendRewardUIState CurRewardState;

	UB2UIFriendKakaoInvite *OwnerUI;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_RewardAmount;

	TWeakObjectPtr<UImage> IMG_RewardItemIcon;

	TWeakObjectPtr<UPanelWidget> X_GetReward;

	TWeakObjectPtr<UB2Button> BTN_Reward;
	TWeakObjectPtr<UTextBlock> TB_GetReward;
	TWeakObjectPtr<UTextBlock> TB_Progress;
	TWeakObjectPtr<UTextBlock> TB_Complete;
};
