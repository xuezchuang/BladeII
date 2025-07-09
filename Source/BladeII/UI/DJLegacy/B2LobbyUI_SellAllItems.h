// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2UIBackWidget.h"
#include "B2UIUtil.h"
#include "B2UIFilterInventory.h"
#include "B2LobbyUI_SellAllItems.generated.h"

#define SELLALLITEMS_STARGRADE_NUM 6 // It is not the same to the maximum star grade.

/**
 * One of Lobby inventory's pop-up. Selling all items except locked and equipped ones.
 */

UCLASS()
class BLADEII_API UB2LobbyUI_SellAllItems : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
protected:
	//////////////////////////////////////////////////
	TWeakObjectPtr<UTextBlock> TB_SellAllTitle;
	TWeakObjectPtr<UTextBlock> TB_GoldToAcquire;
	TWeakObjectPtr<UTextBlock> TB_GoldReceive;
	TWeakObjectPtr<UTextBlock> TB_SellAllNote;
	TWeakObjectPtr<UTextBlock> TB_Sell;
	TWeakObjectPtr<UTextBlock> TB_SellItemCount;
	TWeakObjectPtr<UTextBlock> TB_SellItemNumber;
	TWeakObjectPtr<UTextBlock> TB_SellItemSelect;
	TWeakObjectPtr<UTextBlock> TB_Reset;

	TWeakObjectPtr<UB2Button> BTN_Sell;
	TWeakObjectPtr<UB2Button> BTN_Reset;

	TWeakObjectPtr<UB2Image> IMG_ClassPortrait;

	TWeakObjectPtr<UB2UIFilterInventory> UIP_InventoryFilter;

	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> ClassPortraitImage;

public:
	UB2LobbyUI_SellAllItems(const FObjectInitializer& ObjectInitializer);

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnSellButtonClicked();

protected:
	virtual void CloseWidgetDelegate() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates();

private:
	UFUNCTION()
	void OnAnyItemFilterCBChanged(bool InCheck);

	UFUNCTION()
	void OnReset();

	void UpdateSubWidgets();
	void UpdateStaticWidget(); // To be called once at init stage.

	void UpdateGoldToAcquireText();
	void UpdateSellButtonState();

	/** Get all selling prices of target items which will be sold by current selection. */
	void GetAllTargetItemSellingPrice(int32& ItemCount, int32& ItemPrice);

	void CloseMe();

	bool GetAllTargetItems(TArray<FB2Item> &OutList);
};
