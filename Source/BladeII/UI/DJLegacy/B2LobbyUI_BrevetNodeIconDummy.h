// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2LobbyUI_BrevetNodeIconDummy.generated.h"

/**
 * 
 */

UCLASS()
class BLADEII_API UB2LobbyUI_BrevetNodeIconDummy : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;

public:
	void UpdateStaticText();
	void SetNextRankInfo(UMaterialInterface* pMI, FText& txtRankName);
	void SetUnLockConditionText(int32 nOpenLevel);
	void SetEndRanking();

protected:
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UTextBlock> TB_LeftUnlockCondition;
	TWeakObjectPtr<UTextBlock> TB_RightTip;
	TWeakObjectPtr<UTextBlock> TB_NextUpgrade;
	TWeakObjectPtr<UTextBlock> TB_EndContent;
};
