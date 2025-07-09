// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UICostumeItemInfoPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UICostumeItemInfoPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
	void OnClickBTN_Close();

	UFUNCTION()
	void OnClickBTN_EquipOrRelease();

	UFUNCTION()
	void OnClickBTN_Enhance();

	UFUNCTION()
	void OnClickBTN_Decomposition();

	UFUNCTION()
	void OnClickBTN_ItemLock();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayEquipOrReleaseSound(bool bIsForEquip);

public:
	void InitCostumeItemInfoPopup(const FVector2D& v2PopupPosition, const float& fPopupSizeX, const FB2Item& InItem);
	void UpdateLockState(bool bIsLock, TArray<b2network::B2CostumeServerInfoPtr> ChangeItem);

	void SubscribeEvent();
	void UnsubscribeEvent();

private:
	void UpdateCostumeItemInfoPopup(const FB2Item& InItem);
	void UpdateCostumeItemSlot(const FB2Item& InItem);
	void UpdateCostumeItemOptions(const FB2Item& InItem, const FB2Item* pEquippedItemAtSamePlace);
	void UpdateCostumeConsumableItemOptions(const FB2Item& InItem);
	void UpdateCostumeItemSetEffect(const FB2Item& InItem);
	void UpdateEquipOrReleaseBtnText(const FB2Item& InItem);

	void UpdateButtonsVisibleState();
	void UpdateCombatPowerText(const FB2Item& InItem, const FB2Item* pEquippedItemAtSamePlace);
	void UpdatePrimaryPointText(const FB2Item& InItem, const FB2Item* pEquippedItemAtSamePlace);
	void UpdateOptionList(const EPCClass& ItemAllowedPCClass, const TArray<FItemOption>& ArrayItemOption, const TArray<FItemOption>* ArrayItemOptionAtSamePlace);
	
	void RequestEquipCostumeItem();
	void RequestUnEquipCostumeItem();

	bool GetEquippedItemAtPlace(FB2Item& OutFoundItem, const EItemEquipPlace& InEquipPlace);
	void SetDeltaText(TWeakObjectPtr<UB2RichTextBlock> pRTB, const int32& iDeltaValue, const bool& IsVisible);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIHeaderBezel")
	float fTopBezel_BP;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
	FLinearColor ItemStatIncColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
	FLinearColor ItemStatSameColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
	FLinearColor ItemStatDecColor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UICostumeItemOption")
	TSubclassOf<class UB2UICostumeItemOption>	UICostumeItemOptionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UICostumeItemSetEffect")
	TSubclassOf<class UB2UICostumeItemDetailSetEffect>	UICostumeItemDetailSetEffectClass;
	
private:
	TWeakObjectPtr<UCanvasPanel>	CP_PopupMain;

	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UB2Button>		BTN_EquipOrRelease;
	TWeakObjectPtr<UB2Button>		BTN_Enhance;
	TWeakObjectPtr<UB2Button>		BTN_Decomposition;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_EquipOrReleaseBtn;

	TWeakObjectPtr<class UB2UICostumeItemSlot>	UIP_CostumeItemSlot;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_Combat;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_CombatPower;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_CombatPowerDelta;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_PrimaryPointType;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PrimaryPoint;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PrimaryPointDelta;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_ItemDescription;
	
	TWeakObjectPtr<UTextBlock>			STB_EnhanceBtn;
	TWeakObjectPtr<UTextBlock>			STB_Decomposition;
	TWeakObjectPtr<UTextBlock>			TB_ItemDesc;

	TWeakObjectPtr<UVerticalBox>		VB_OptionsDisplayPanel;

	TWeakObjectPtr<UVerticalBox>		VB_SetEffectList;

	TWeakObjectPtr<UVerticalBox>		VB_LeftButtonSet;

	TWeakObjectPtr<class UB2UICostumeOpenBtn>	UIP_CostumeOpenLinkBtn;

	TWeakObjectPtr<UPanelWidget>		X_P_ItemLockButtonSet;
	TWeakObjectPtr<UButton>				BTN_ItemLock;
	TWeakObjectPtr<UWidgetSwitcher>		WS_Lock;

	TWeakObjectPtr<UPanelWidget>		X_P_ItemInfoDisplayPanel;
	TWeakObjectPtr<UPanelWidget>		X_P_ItemDescriptionPanel;

	TWeakObjectPtr<UPanelWidget>		P_CostumeLinkBtn;
private:
	FB2Item CachedB2ItemInfo;
	TArray<std::shared_ptr<class issue>> Issues;
};
