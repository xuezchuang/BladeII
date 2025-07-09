// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2LobbyUI_AutoReinforce.generated.h"


#define AutoReinfALLITEMS_STARGRADE_NUM 6

/**
* One of Lobby inventory's pop-up. 
*/

UCLASS()
class BLADEII_API UB2LobbyUI_AutoReinforce : public UB2LobbyUserWidget
{
	GENERATED_BODY()

private:
	EItemInvenType SelectedAutoReinfTab;
	UCheckBox* CB_ItemGrade[AutoReinfALLITEMS_STARGRADE_NUM];

	int32 CachedLastPreviewAutoEnhanceCost;

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnCloseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickTapSelect(EItemInvenType InTabSelection);

	void SetSelectedAutoReinfTab(EItemInvenType InSelection);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnClickAutoReinforceCommit();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		ESlateVisibility GetWeaponVisibility() const;
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		ESlateVisibility GetProtectionVisibility() const;
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		ESlateVisibility GetAccessoryVisibility() const;

	/** Callback for any one of item filter checkbox state change */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnAnyItemFilterCBChanged();
	

	UB2LobbyUI_AutoReinforce(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void UpdateOnAutoItemLevelupCostDelivered(int32 DeliveredCost);

private:
	ESlateVisibility GetAutoReinfTabSelectedImageVisibilityCommon(EItemInvenType InTabSelection) const;
	
	void UpdateStaticTexts();

	/** It returns all item list by current selection. */
	void GetItemListByFilter(TArray<FB2Item>& OutFilteredList);

	void GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter);
	void CloseMe();

	void FreezeSelectionInterface(bool bFreeze);

	void UpdateAutoReinforceCommitBtnEnable(int32 AutoReinforceCost, bool bSelectionFreezed);

protected:
	TWeakObjectPtr<UTextBlock>	TB_AutoReinfTitle;
	TWeakObjectPtr<UTextBlock>	TB_AutoRef;
	TWeakObjectPtr<UTextBlock>	TB_GoldToAcquire;
	TWeakObjectPtr<UTextBlock>	TB_Weapon;
	TWeakObjectPtr<UTextBlock>	TB_Protection;
	TWeakObjectPtr<UTextBlock>	TB_Accessory;
	TWeakObjectPtr<UTextBlock>	TB_ItemGrade;
	TWeakObjectPtr<UB2Button>	BTN_Weapon;
	TWeakObjectPtr<UB2Button>	BTN_WeaponSelect;
	TWeakObjectPtr<UB2Button>	BTN_Protection;
	TWeakObjectPtr<UB2Button>	BTN_ProtectionSelect;
	TWeakObjectPtr<UB2Button>	BTN_Accessory;
	TWeakObjectPtr<UB2Button>	BTN_AccessorySelect;
	TWeakObjectPtr<UCheckBox>	CB_ItemGrade_1;
	TWeakObjectPtr<UCheckBox>	CB_ItemGrade_2;
	TWeakObjectPtr<UCheckBox>	CB_ItemGrade_3;
	TWeakObjectPtr<UCheckBox>	CB_ItemGrade_4;
	TWeakObjectPtr<UCheckBox>	CB_ItemGrade_5;
	TWeakObjectPtr<UCheckBox>	CB_ItemGrade_6;
	TWeakObjectPtr<UB2Button>	Btn_Close;
	TWeakObjectPtr<UB2Button>	Btn_AutoReinforceCommit;

	TWeakObjectPtr<UTextBlock> TB_AutoEnhanceNote_1;
	TWeakObjectPtr<UTextBlock> TB_AutoEnhanceNote_2;
};
