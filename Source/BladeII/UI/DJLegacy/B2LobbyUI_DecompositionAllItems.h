// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2UIBackWidget.h"
#include "B2UIUtil.h"
#include "B2UIFilterInventory.h"
#include "../Widget/B2Image.h"
#include "B2LobbyUI_DecompositionAllItems.generated.h"

#define DECOMPOSITION_ALLITEMS_STARGRADE_NUM 7


UCLASS()
class BLADEII_API UB2LobbyUI_DecompositionAllItems : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	//////////////////////////////////////////////////
	TWeakObjectPtr<UTextBlock> TB_DecompostionAllTitle;

	TArray<TWeakObjectPtr<UHorizontalBox>> HB_Decomposition;
	TArray<TWeakObjectPtr<UImage>> IMG_Decomposition;
	TArray<TWeakObjectPtr<UTextBlock>> TB_DecompositionCount;
	TWeakObjectPtr<UTextBlock> TB_DecompositionReceive;

	TWeakObjectPtr<UTextBlock> TB_DecompostionAllNote;
	TWeakObjectPtr<UTextBlock> TB_Decompostion;
	TWeakObjectPtr<UTextBlock> TB_DecompostionItemCount;
	TWeakObjectPtr<UTextBlock> TB_DecompostionItemNumber;
	TWeakObjectPtr<UTextBlock> TB_DecompostionItemSelect;
	TWeakObjectPtr<UTextBlock> TB_Reset;

	TWeakObjectPtr<UB2Button> BTN_Decompostion;
	TWeakObjectPtr<UB2Button> BTN_Reset;

	TWeakObjectPtr<UB2Image> IMG_ClassPortrait;

	TWeakObjectPtr<UB2UIFilterInventory> UIP_InventoryFilter;

	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> ClassPortraitImage;

public:
	UB2LobbyUI_DecompositionAllItems(const FObjectInitializer& ObjectInitializer);

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnDecompositionButtonClicked();

protected:
	virtual void CloseWidgetDelegate() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates();

private:
	UFUNCTION()
	void OnAnyItemFilterCBChanged(bool InCheck);

	UFUNCTION()
	void OnReset();

private:
	void UpdateSubWidgets();
	void UpdateStaticWidget();

	void UpdateDecompositionObtainState();
	void UpdateDecompositionButtonState();

	void GetAllTargetItemObtainDecomposition(int32& InItemCount, TMultiMap<int32, int32>& InDismantleItems, TArray<int32>& InDismantleItemsKey);

	void CloseMe();

	bool GetAllTargetItems(TArray<FB2Item>& OutList);
};
