// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIBattleStageInfoToolTip.generated.h"

/**
 * 
 */
UCLASS()
class UB2UIBattleStageInfoToolTip : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void ShowRewardItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void ShowBossPieceInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void ShowEtherItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void ShowMaterialItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);

	UFUNCTION()
		void HideToolTip();

protected:
	virtual void CloseWidgetDelegate() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	void ShowToolTip();

	TWeakObjectPtr<UImage>							IMG_InfoGoodsIcon;
	TWeakObjectPtr<UTextBlock>						TB_RewardItemName;
	TWeakObjectPtr<UTextBlock>						TB_RewardItemOptionText;
	TWeakObjectPtr<UTextBlock>						TB_RewardItemDesc;
	TWeakObjectPtr<UB2Button>						BTN_HideRewardItemInfoSet;
};
