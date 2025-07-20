// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"

#include "EventSubsystem.h"
#include "B2UIMsgPopupSelectFairyGift.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMsgPopupSelectFairyGift : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OnChangedFairy(EFairyType FairyType);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClick_Confirm();
	UFUNCTION()
		void OnClick_Close();

private:
	TArray<std::shared_ptr<issue>> Issues;

	EFairyType SelectedFairy;

	TWeakObjectPtr<UTextBlock>	TB_Title;
	TWeakObjectPtr<UTextBlock>	TB_GiftChoice;
	TWeakObjectPtr<UTextBlock>	TB_Confirm;

	TWeakObjectPtr<UB2Button>	BTN_Confirm;
	TWeakObjectPtr<UB2Button>	BTN_Close;

	TWeakObjectPtr<class UB2UIFairyGiftButton> UIP_FairyGiftButton_Niece;
	TWeakObjectPtr<class UB2UIFairyGiftButton> UIP_FairyGiftButton_Pang;
	TWeakObjectPtr<class UB2UIFairyGiftButton> UIP_FairyGiftButton_Rio;

	TMap<EFairyType, TWeakObjectPtr<class UB2UIFairyGiftButton>> UIP_FairyGiftButtons;
};
