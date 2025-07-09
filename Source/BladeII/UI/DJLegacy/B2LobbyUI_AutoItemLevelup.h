// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2UIBackWidget.h"
#include "B2UIFilterInventory.h"
#include "B2UIUtil.h"
#include "B2LobbyUI_AutoItemLevelup.generated.h"


#define AUTOLEVELUP_ALLITEMS_STARGRADE_NUM 6

/**
* One of Lobby inventory's pop-up. 
*/

UCLASS()
class BLADEII_API UB2LobbyUI_AutoItemLevelup : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<UTextBlock>	TB_AutoLevelupTitle;
	TWeakObjectPtr<UTextBlock>	TB_AutoLevelUp;
	TWeakObjectPtr<UTextBlock>	TB_GoldToAcquire;
	
	TWeakObjectPtr<UTextBlock>	TB_LevelUpItemNumber;
	TWeakObjectPtr<UTextBlock>	TB_LevelUpItemCount;

	TWeakObjectPtr<UTextBlock>	TB_IngredItemNumber;
	TWeakObjectPtr<UTextBlock>	TB_IngredItemCount;

	TWeakObjectPtr<UTextBlock>	TB_LVUP;
	TWeakObjectPtr<UTextBlock>	TB_SelectIngred;
	TWeakObjectPtr<UTextBlock>	TB_Reset;

	TWeakObjectPtr<UB2Button>	Btn_Close;
	TWeakObjectPtr<UB2Button>	Btn_AutoLevelupCommit;
	TWeakObjectPtr<UB2Button>	Btn_Reset;

	TWeakObjectPtr<UTextBlock> TB_AutoLevelupNote_1;
	TWeakObjectPtr<UTextBlock> TB_AutoLevelupNote_2;

	TWeakObjectPtr<UB2Image> IMG_ClassPortrait;

	TWeakObjectPtr<UB2UIFilterInventory> UIP_InventoryFilter;

	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> ClassPortraitImage;

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnClickReset();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnClickAutoLevelUpCommit();

	UFUNCTION()
	void OnAnyItemFilterCBChanged(bool bCheck);
	
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

protected:
	virtual void CloseWidgetDelegate() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates();

private:
	void CloseMe();

	void UpdateWidgets();
	void UpdateStaticWidgets();

	/** It returns all item list by current selection. */
	void GetItemListByFilter(TArray<FB2Item>& OutFilteredList);
};
