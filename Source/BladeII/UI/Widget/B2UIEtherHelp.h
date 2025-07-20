// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIEtherHelp.generated.h"


/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherHelp : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ether")
		TSubclassOf<class UB2UIEtherHelpSlot> HelpSlotClass;

protected:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickClose();

	TWeakObjectPtr<UB2Button>																	X_BTN_Close;
	TWeakObjectPtr<UTextBlock>																	X_TB_Title;
	TWeakObjectPtr<UB2RichTextBlock>															X_RTB_DefenceSet;
	TWeakObjectPtr<UB2RichTextBlock>															X_RTB_DefenceSetDesc;
	TWeakObjectPtr<UB2RichTextBlock>															X_RTB_OffenseSet;
	TWeakObjectPtr<UB2RichTextBlock>															X_RTB_OffenseSetDesc;
	TWeakObjectPtr<UB2ScrollBox>																X_SB_Scroll;
	TArray<TWeakObjectPtr<class UB2UIEtherHelpSlot>>											UIP_HelpSlots;
};
