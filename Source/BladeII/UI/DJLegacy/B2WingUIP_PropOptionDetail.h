// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_PropOptionIcon.h"
#include "B2WingUIP_PropOptionDetail.generated.h"

/**
 * A part of prop option enhancement menu, providing enhancement interface as well as displaying detailed prop option info.
 */
UCLASS()
class BLADEII_API UB2WingUIP_PropOptionDetail : public UBladeIIUserWidget
{
	GENERATED_BODY()

	// Widget switcher index define
	static const int32 BGWSIndex_Locked;
	static const int32 BGWSIndex_NormalOpen;
	static const int32 BGWSIndex_AtMaxLevel;

protected:

	int32 CachedOptionIndex;
	FB2Wing CachedWingData; // Required for ref data lookup
	FWingPropOption CachedOptionData; // The main information displayed by this icon.

	TWeakObjectPtr<UB2WingUIP_PropOptionIcon> UIP_PropOptionIcon;

	TWeakObjectPtr<UTextBlock> TB_OptionName;
	TWeakObjectPtr<UTextBlock> TB_OptionValue;
	TWeakObjectPtr<UTextBlock> TB_OptionLevel;
	TWeakObjectPtr<UTextBlock> TB_OptionMaxLevelInfo;

	// You can put any stuff that need to be turned on only for some state, by BGWSIndex_** defines here.
	TWeakObjectPtr<UWidgetSwitcher> WS_PerOptionStateSet;

	// They might just going in WS_PerOptionStateSet 
	TWeakObjectPtr<UOverlay> OV_EnhanceBtnSet;
		TWeakObjectPtr<UB2Button> BTN_Enhance;
		TWeakObjectPtr<UTextBlock> TB_DoEnhance;
		TWeakObjectPtr<UTextBlock> TB_RequiredFeatherNum;
		
	TWeakObjectPtr<UB2Button> BTN_DoEnhance;
	TWeakObjectPtr<UImage> IMG_Feather;

	TWeakObjectPtr<UOverlay> OV_AtMaxLevelSet;
		TWeakObjectPtr<UTextBlock> TB_AtMaxLevel;

	TWeakObjectPtr<UB2RichTextBlock> TB_UnlockInfo;

	TWeakObjectPtr<UProgressBar> PB_EnhanceProgress;
	TWeakObjectPtr<UTextBlock> TB_EnhanceProgress;

	TWeakObjectPtr<UOverlay> OV_AtNextLevelSet;
		TWeakObjectPtr<UTextBlock> TB_AtNextLevel;

public:

	UB2WingUIP_PropOptionDetail(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;
		
	void SetMyOptionInfo(int32 InMyOptionIndex, const FB2Wing& InWingData, const FWingPropOption& InOptionData); // WingData is needed for reference data lookup.

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlaySuccessAnim_BP();
private:
	void UpdateWidgets();

	/** Get some ref data and see if user can enhance current prop option by return value. */
	bool GetEnhanceRequirementAndFeasibility(int32& OutRequiredFeatherNum, int32& OutCurrentFeatherNum);

	UFUNCTION()
	void OnClickedEnhanceBtn();
};

