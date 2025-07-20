// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2UIBackWidget.h"
#include "B2DynItemIcon_LobbyEquip.h"
#include "../../DataStore/B2TotemData.h"
#include "B2LobbyUI_SellItemPopup.generated.h"

DECLARE_DELEGATE(FSellItemPopupOnClick)

/**
 * Popup to confirm item selling. It is almost MsgPopup, but has icon..
 */
UCLASS()
class BLADEII_API UB2LobbyUI_SellItemPopup : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	FB2Item ItemToSell; // Would not be valid when bIsForSellAllResult is true.
	uint32 bIsForSellAllResult : 1;
	int32 SellAllResultGold; // Required only when bIsForSellAllResult is true;

protected:
	virtual void CloseWidgetDelegate() override;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_AskForSelling;
	TWeakObjectPtr<UTextBlock> TB_TotalGold;
	TWeakObjectPtr<UTextBlock> TB_Confirm;

	TWeakObjectPtr<UTextBlock>	TB_ItemSellMax;
	TWeakObjectPtr<UTextBlock>	TB_ItemSell;

	TWeakObjectPtr<UTextBlock> TB_CancelBtn;
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UB2Button> BTN_Cancel;

	TWeakObjectPtr<UB2Button> BTN_MaxItemCount;
	TWeakObjectPtr<UB2Button> BTN_PlusItemCount;
	TWeakObjectPtr<UB2Button> BTN_MinusItemCount;
	TWeakObjectPtr<USlider>	  SD_ItemCount;
	TWeakObjectPtr<UTextBlock> TB_ItemCount;
	TWeakObjectPtr<UTextBlock> TB_MaxItemCount;

public:
	UB2LobbyUI_SellItemPopup(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
		
	// Sell Item
	void PopupMain(const FB2Item& InItemToSell, const FSellItemPopupOnClick& InConfirmBtnHandler, const FSellItemPopupOnClick& InCancelBtnHandler);
	void PopupAsSellResult(const int32 AcquiredGold);

	// Sell Ether
	void PopupMain(const int32 EtherPrice, const FSellItemPopupOnClick& InConfirmBtnHandler, const FSellItemPopupOnClick& InCancelBtnHandler);
	void PopupAsSellEtherResult(const int32 AcquiredGold, const FSellItemPopupOnClick& InConfirmBtnHandler);

	// Sell Totem
	void PopupMain(const FB2Totem& InTotemToSell, const FSellItemPopupOnClick& InConfirmBtnHandler, const FSellItemPopupOnClick& InCancelBtnHandler);
	void PopupAsSellTotemResult(const int32 AcquiredGold);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnChangeItemCountSliderBar(float SliderValue);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float ConvertItemCountToSliderValue(int32 InItemCount);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 ConvertSliderValueToItemCount(float InSliderValue);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetItemCountAndUpdateWidget(int32 InItemCount);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetSellItemCount();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetNextSellItemCount();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetPreSellItemCount();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void HiddenSelectWidgets_BP();

private:
	void UpdateStaticWidgets();
	void UpdateDynamicWidgets();
	void UpdateSellItemWidgets();
	void UpdateSellEtherWidgets();

	void SetItemCount(int32 InItemCount);
	void SetMaxItemCount(int32 InMaxCount, int32 TotalItemCount);
	void SetMaxItemCount(int32 InMaxCount);
	void SetElementSize(int32 InMaxCount);

	int32 GetMaxItemCount();
	float GetDivideElement();
private:
	UFUNCTION()
	void OnClickConfirmBtn();

	UFUNCTION()
	void OnClickCancelBtn();

	UFUNCTION()
	void OnClickMaxItemCountBtn();

	void CloseMe();

private:
	FSellItemPopupOnClick OnClickConfirmBtnHandler;
	FSellItemPopupOnClick OnClickCancelBtnHandler;

	int32 MaxItemCount;
	int32 SellItemCount;

	float ElementSize;

	const int64 MaxGoldAmount = 2100000000;
};
