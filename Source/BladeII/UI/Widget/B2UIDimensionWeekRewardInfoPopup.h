// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDimensionWeekRewardInfoPopup.generated.h"


UCLASS()
class BLADEII_API UB2UIDimensionWeekRewardInfoPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	UFUNCTION()
	void OnClickBTN_Close();

private:
	TWeakObjectPtr<UTextBlock> TB_WeeklyReward;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TArray<TWeakObjectPtr<class UB2UIDimensionWeekRewardInfoItem>> RewardSlots;
};


UCLASS()
class BLADEII_API UB2UIDimensionWeekRewardInfoItem : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetItemInfo(bool bIsUseSub, int32 nMinRank, int32 nMaxRank, int32 nIndex1, int32 nCount1, int32 nIndex2 = 0, int32 nCount2 = 0);

private:
	TWeakObjectPtr<UWidgetSwitcher>		WS_Item;
	TWeakObjectPtr<UTextBlock>			TB_Rank;
	TWeakObjectPtr<UTextBlock>			TB_Count;
	TWeakObjectPtr<UTextBlock>			TB_SubCount;
	TWeakObjectPtr<UTextBlock>			TB_OnlySubCount;
	TWeakObjectPtr<UTextBlock>			TB_ItemName;
	TWeakObjectPtr<UImage>				IMG_RewardMainIcon;
	TWeakObjectPtr<UImage>				IMG_RewardSubIcon;
	TWeakObjectPtr<UImage>				IMG_OnlySub; 
	TWeakObjectPtr<class UB2ButtonGoodInfoToolTip>				BTN_ToolTip_Main;
	TWeakObjectPtr<class UB2ButtonGoodInfoToolTip>				BTN_ToolTip_Sub;
	TWeakObjectPtr<class UB2ButtonGoodInfoToolTip>				BTM_ToolTip_OnlySub;
};