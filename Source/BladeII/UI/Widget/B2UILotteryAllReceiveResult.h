// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UILotteryAllReceiveResult.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UILotteryAllReceiveResult : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void UpdateResultItemIcons(TArray<FB2Item>& InResultItems);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnCloseButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnConfirmBtnClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		TSubclassOf<class UB2DynItemIconRow_LobbyInven> ItemIconRowWidgetClass;
	
	UPROPERTY(Transient)
		TArray<TWeakObjectPtr<class UB2DynItemIconRow_LobbyInven>> CreatedItemRows;

	TWeakObjectPtr<UScrollBox> SB_ItemDisplayPanel;
	TWeakObjectPtr<UTextBlock> TB_AutoEnhanceResultTitle;
	TWeakObjectPtr<UTextBlock> TB_ConfirmBtn;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TArray<FB2Item> ResultItems;
};
