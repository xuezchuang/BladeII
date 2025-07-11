// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "B2LobbyUserWidget.h"
//#include "EventSubsystem.h"
//#include "B2UIBackWidget.h"
//#include "B2LobbyUI_TotemInventoryMain.generated.h"

///**
// * 
// */
//UCLASS()
//class BLADEII_API UB2LobbyUI_TotemInventoryMain : public UB2LobbyUserWidget, public IB2UIBackWidget
//{
//	GENERATED_BODY()
//
//public:
//	UB2LobbyUI_TotemInventoryMain(const FObjectInitializer& ObjectInitializer);
//
//	void Init();
//
//	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
//	virtual void DestroySelf() override;
//
//	virtual void DoMarkRedDot() final;
//
//	void UpdateStoredTotem();
//	void DestroyAllTotemIcons();
//
//protected:
//	virtual void CloseWidgetDelegate() override;
//
//private:
//	virtual void CacheAssets() override;
//
//	void BindDelegates();
//	void UpdateStaticText();
//
//	void InitTotemScrollOffset();
//
//	void SubscribeEvents();
//	void UnsubscribeEvents();
//
//	void SortTotemSlot(TArray<FB2Totem>& InOutSortingTotems);
//	
//	void CloseAllStandaloneUIFromUIManager();
//
//public:
//	UCanvasPanel* GetItemDetailPopupParentPanel(bool bOfLeftSide) const;
//	UOverlay* GetItemDetailPopupParentFullPanel() const;
//
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		FText GetContainerSizeText() const;
//
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		FText GetStoredItemNumText() const;
//
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		FSlateColor GetStoredItemNumTextColor() const;
//
//private:
//	UFUNCTION()
//		void OnClickBTN_AutoDecomposition();
//
//	UFUNCTION()
//		void OnClickBTN_SellAll();
//
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TotemSlot")
//		FSlateColor StoredTotemNumTextColor_Normal;
//
//	/** Applied when stored item over the limit of lobby inventory. We got some occasion for that. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TotemSlot")
//		FSlateColor StoredTotemNumTextColor_Over;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		TSubclassOf<class UB2LobbyUI_TotemInventorySlot>	TotemSlotClass;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		int32												iHorizontalMax;
//
//private:
//	TWeakObjectPtr<UTextBlock>			TB_TotemInventory;
//
//	TWeakObjectPtr<UTextBlock>			TB_AutoDecomposition;
//	TWeakObjectPtr<UTextBlock>			TB_SellAll;
//
//	TWeakObjectPtr<UB2Button>			BTN_AutoDecomposition;
//	TWeakObjectPtr<UB2Button>			BTN_SellAll;
//
//	TWeakObjectPtr<class UCanvasPanel>	X_LeftPopupPanel;
//	TWeakObjectPtr<class UCanvasPanel>	X_RightPopupPanel;
//
//	TWeakObjectPtr<class UB2LobbyUI_HeroMgmtCommon> HeroMgmtCommonBP;
//	TWeakObjectPtr<class UB2ScrollBoxWithGridPanel>	UIP_ScrollBoxWithGridPanelBP;
//
//	TWeakObjectPtr<class UB2LobbyUI_TotemEquipSlotList>	UIP_TotemEquipSlotList;
//
//	TArray<TWeakObjectPtr<UB2LobbyUI_TotemInventorySlot>>	ArrayTotemSlot;
//
//private:
//	TArray<issue_ptr>				Issues;
//};
