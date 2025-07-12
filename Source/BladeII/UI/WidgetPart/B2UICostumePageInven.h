// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../Common/CommonStruct.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "B2UICostumePageInven.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UICostumePageInven : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	
	virtual void RegisterUIMarkForRedDot() final;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
	void OnClickBTN_TabWeapon();

	UFUNCTION()
	void OnClickBTN_TabArmors();

	UFUNCTION()
	void OnClickBTN_TabMaterial();

	UFUNCTION()
	bool HasRedDot_TabWeapon();

	UFUNCTION()
	bool HasRedDot_TabArmors();

	UFUNCTION()
	bool HasRedDot_TabMaterial();

	UFUNCTION()
	void DeliveryModifyCostumeNewFlag();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void InitCostumePageInven();
	void InitCostumeItemList();
	void InitScrollOffset();

	void UpdateTabBtnState(const int32& iTabIndex);
	void UpdateCostumeInvenFromCurrentPCClass();
	void UpdateCostumeInvenFromSelectionPCClass(EPCClass SelectionPCClass);
	void UpdateCostumeInvenFromMaterials();

	void CloseAllStandaloneUIFromUIManager();

	//reddot
	void RedDotCondition_TapState();
	void RequestModifyCostumeNewFlag();
	void RequestModifyCostumeNewFlag(EPCClass CurrentClass, EItemInvenType InvenType);

	FText GetContainerSizeText() const;
	FText GetStoredItemNumText() const;
	FSlateColor GetStoredItemNumTextColor() const;
	
	void UpdateCostumeInvenItemCounter(bool bVisible);
	void ForcedTabClickMaterial(bool bFreezeTab);

public:
	void OnCallBackEnhance(const FB2Item& InItem);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UICostumeItemSlot>		CostumeItemSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32										iHorizontalMax;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	FSlateColor StoredItemNumTextColor_Normal;
	
	/** Applied when stored item over the limit of lobby inventory. We got some occasion for that. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	FSlateColor StoredItemNumTextColor_Over;

private:
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	//btntab_weapon
	TWeakObjectPtr<UCanvasPanel>	CP_TabWeapon;
	TWeakObjectPtr<UB2Button>		BTN_TabWeapon;
	TWeakObjectPtr<UWidgetSwitcher>	WS_TabWeapon;
	TWeakObjectPtr<UTextBlock>		TB_UnSelectedTabWeapon;
	TWeakObjectPtr<UTextBlock>		TB_SelectedTabWeapon;
	//btntab_weapon

	//btntab_armor
	TWeakObjectPtr<UCanvasPanel>	CP_TabArmors;
	TWeakObjectPtr<UB2Button>		BTN_TabArmors;
	TWeakObjectPtr<UWidgetSwitcher>	WS_TabArmors;
	TWeakObjectPtr<UTextBlock>		TB_UnSelectedTabArmors;
	TWeakObjectPtr<UTextBlock>		TB_SelectedTabArmors;
	//btntab_armor

	//btntab_material
	TWeakObjectPtr<UCanvasPanel>	CP_TabMaterial;
	TWeakObjectPtr<UB2Button>		BTN_TabMaterial;
	TWeakObjectPtr<UWidgetSwitcher>	WS_TabMaterial;
	TWeakObjectPtr<UTextBlock>		TB_UnSelectedTabMaterial;
	TWeakObjectPtr<UTextBlock>		TB_SelectedTabMaterial;
	//btntab_material

	TWeakObjectPtr<UTextBlock>		StoredItemNumText;
	TWeakObjectPtr<UTextBlock>		ContainerSizeText;
	TWeakObjectPtr<UTextBlock>		SlashText;
	
	TWeakObjectPtr<class UB2ScrollBoxWithGridPanel>	UIP_ScrollBoxWithGridPanelBP;

	TArray<TWeakObjectPtr<UB2UICostumeItemSlot>>	ArrayCostumeItemSlot;
	EItemInvenType									CurSelectedItemInvenTab;
	
	EItemInvenType									BeforeCurSelectedItemInvenTab;
	EPCClass										BeforeCurSelectedPCClass;

	EPCClass										LastSelectedPCClass;

private:
	TArray<issue_ptr>				Issues;
};
