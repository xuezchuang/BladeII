// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIEtherLotterySlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLotterySlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetEtherLotterySlotInfo(int32 InTypeID, int32 InStar, int32 InTier);
	void SetSelectedVisibility(int32 InType);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Bladeii")
		void TouchEtherLotterySlotPlaySound();

	virtual void CacheAssets() override;
	virtual void OnClickedSenderBTNArea() override;

private:
	void SetEtherInfo(int32 InTypeID, int32 InTier);
	void SetTierInfo(int32 InTier);
	void SetStarInfo(int32 InStar);
	
private:
	TWeakObjectPtr<UImage> X_IMG_SetImage;
	TWeakObjectPtr<UImage> X_IMG_TierBG;
	TWeakObjectPtr<UTextBlock> X_TB_EtherTierName;
	TWeakObjectPtr<UTextBlock> X_TB_EtherName;
	TWeakObjectPtr<UImage> X_IMG_BG;
	TWeakObjectPtr<UPanelWidget> PW_Selected;
	int32 TypeID;
	TWeakObjectPtr<class UB2UIStarGradePart> UIP_Star;
};
