// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "../B2RichTextBlock.h"

#include "B2UIEtherLotteryPopupBase.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLotteryPopupBase : public UB2UIWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	void SetEtherSetInfo(b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr);
	void SetTierInfo(int32 InTier);

protected:
	struct FSetInfo
	{
		TWeakObjectPtr<UTextBlock>	TB_Name;
		TWeakObjectPtr<UTextBlock>	TB_Value;
		TWeakObjectPtr<UPanelWidget> PW_Slot;

		void SetText(FString InNameKey, FString InOptionKey, float InValue);
	};

	TWeakObjectPtr<UB2Button> X_BTN_Close;
	TWeakObjectPtr<class UB2UIEtherScrollSlot> UI_EtherIcon;
	TArray<TWeakObjectPtr<class UB2UIEtherOptionSlot>> UIP_EtherOptions;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MainOptionName;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MainOptionValue;

private:
	TWeakObjectPtr<UTextBlock> X_TB_SetName;
	TWeakObjectPtr<UTextBlock> X_TB_SetInfo;
	TWeakObjectPtr<UImage> X_ING_TierBG;
	TWeakObjectPtr<UTextBlock> X_TB_EtherTierName;
	TArray<FSetInfo> UIP_EtherSetOptionInfos;
};
