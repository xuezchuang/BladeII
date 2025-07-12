// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FairyManager.h"
#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIFairyBlessSlot.generated.h"

DECLARE_DELEGATE_OneParam(FFairyBlessSlotOnClick, int32)
DECLARE_DELEGATE_OneParam(FFairyBlessIconOnClick, int32)

namespace EFairyBlessSlotStatus
{
	enum Type : uint8
	{
		Disabled,
		Enabled,
		Selected
	};
}

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyBlessSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetFairyBlessSlotHandler(const FFairyBlessSlotOnClick& InBtnHandler);
	void SetFairyBlessIconHandler(const FFairyBlessIconOnClick& InBtnhandler);

	void SetBlessSlotInfo(int32 RowIndex, const FB2FairyBlessSlotInfo &Info);

	void SetSlotStaus(EFairyBlessSlotStatus::Type Status);

	int32 GetRowIndex() { return CachedRowIndex; }

	UFUNCTION(BlueprintCallable)
	void StartBlessAnimation();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	void SetEnabledStatus();
	void SetDisabledStatus();
	void SetSelectedStatus();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBlessAnimation();

	UFUNCTION(BlueprintCallable)
	void OnFinishedBlessAnimation();

	UFUNCTION()
	void OnClickBTN_BlessSlot();

private:
	int32 CachedRowIndex;
	bool bEnabledClick;
	bool bIsBless;

	EFairyBlessSlotStatus::Type CurrentSlotStatus;

	FFairyBlessSlotOnClick OnBlessSlotClickHandler;
	FFairyBlessIconOnClick OnBlessIconClickHandler;

	TWeakObjectPtr<UPanelWidget> P_Disable;
	TWeakObjectPtr<UPanelWidget> P_Able;
	TWeakObjectPtr<UPanelWidget> P_Selected;

	TWeakObjectPtr<UPanelWidget> OV_FXBlessing;
	TWeakObjectPtr<UPanelWidget> OV_FXBlessON;

	TWeakObjectPtr<UOverlay> OV_Detail;

	TWeakObjectPtr<UImage> IMG_BelssIcon;
	TWeakObjectPtr<UImage> IMG_LockBG;

	TWeakObjectPtr<UB2Button> BTN_BelssSelect;

	TWeakObjectPtr<UTextBlock> TB_Bless;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Desc;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Count;
	TWeakObjectPtr<UTextBlock> TB_FairyOpen;

	TWeakObjectPtr<UPanelWidget> OV_Lock;
	TWeakObjectPtr<UPanelWidget> OV_Check;

};
