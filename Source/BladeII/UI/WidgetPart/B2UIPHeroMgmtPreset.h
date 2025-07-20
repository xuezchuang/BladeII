// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIPHeroMgmtPreset.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPresetUsageType : uint8
{
	Select,
	Preview,
	End
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPHeroMgmtPreset : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	void SetUsageType(EPresetUsageType UsageType);
	void SetSlotNumber(EPCClass SelectPCClass, EItemPresetSlotType SlotNumber);

	void SwitchState(bool bEnabled);

	bool ShowChangeMessage();
	void ShowSlotPressedMessage();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	UFUNCTION()
	void OnPressedArea();

protected:
	TWeakObjectPtr<UImage> IMG_Select;
	TWeakObjectPtr<UImage> IMG_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Select;
	TWeakObjectPtr<UTextBlock> TB_Unselect;
	TWeakObjectPtr<UB2Button> BTN_Area;

private:
	EPCClass SelectedHeroMgmtPCClass;
	EItemPresetSlotType PresetSlot;
	EPresetUsageType PresetUsageType;

	bool IsPressed;
};
