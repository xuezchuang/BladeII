// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "FairyManager.h"
#include "../B2RichTextBlock.h"
#include "B2UIFairyMain.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFairyMain : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void RefreshFairyStatus();

	void RefreshSelectedFairyInfo();
	void RefreshFairyButtons();
	void RefreshBottomButtons();

	void OnChangedFairy(EFairyType Fairy);
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	void RefreshFairyGift();
	void RefreshFairyBlessInfo();
	void UpdateLevelStepIcons();

	void OnReceivedBless();

	void OnUpdateFairyBless(EFairyType FairyType);

	void OnFairyBlessSlotCallback(int32 RowIndex);
	void OnFairyBlessSlotIconCallback(int32 RowIndex);

private:
	void SubscribeEvent();
	void UnSubscribeEvent();
	bool bSubscribed;
	TArray<std::shared_ptr<issue>> Issues;

protected:
	UFUNCTION()
	void OnClickBTN_GoToInven();
	UFUNCTION()
	void OnClickBTN_GoToLevelUp();
	UFUNCTION()
	void OnClickBTN_GoToBless();
	UFUNCTION()
	void OnClickBTN_DailyReward();

	void OnChangeAutoLevelUp(bool IsRunning);
protected:

	UPROPERTY()
	TSubclassOf<class UB2UIFairyLevelUpPopup> OnLevelUpWidgetClass;
private:	

	bool bLoopReceiveAutoLevelUp;

	EFairyType CurrentFairyType;

	int32 CachedBlessSlotRowIndex;

	TArray<struct FB2FairyBlessSlotInfo> CachedFairyBlessList;

	TWeakObjectPtr<UGridPanel> X_BelssItemList;
	TWeakObjectPtr<UB2Button> BTN_GoToInven;
	TWeakObjectPtr<UTextBlock> STB_GoToInven;
	TWeakObjectPtr<UB2Button> BTN_GoToLevelUp;
	TWeakObjectPtr<UTextBlock> STB_GoToLevelUp;
	TWeakObjectPtr<UB2Button> BTN_GoToBless;
	TWeakObjectPtr<UTextBlock> STB_GoToBless;

	TWeakObjectPtr<UTextBlock> TB_Title;

	TWeakObjectPtr<UTextBlock> TB_FairyName;
	TWeakObjectPtr<UTextBlock> TB_FairyLevel;
	TWeakObjectPtr<UTextBlock> TB_NextLevel;
	TWeakObjectPtr<UB2RichTextBlock> TB_NextOption;

	TWeakObjectPtr<class UB2UIFairyButton> BTN_Fairy_Niece;
	TWeakObjectPtr<class UB2UIFairyButton> BTN_Fairy_Pang;
	TWeakObjectPtr<class UB2UIFairyButton> BTN_Fairy_Rio;

	TWeakObjectPtr<class UB2UIFairyLevelUpPopup> UIP_FairyLevelUpPopup;

	TArray<TWeakObjectPtr<class UB2UIFairyBlessSlot>> FairyBlessSlotList;

	TArray<TWeakObjectPtr<class UB2UIFairyStepIcon>> FairyStepIconList;

	TWeakObjectPtr<UOverlay> P_DailyReward;
	TWeakObjectPtr<UOverlay> P_DailyRewardOn;
	TWeakObjectPtr<UB2Button> BTN_DailyReward;
};
