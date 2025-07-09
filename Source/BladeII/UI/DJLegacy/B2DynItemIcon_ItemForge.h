// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DJLegacy/B2DynItemIcon.h"
#include "B2UIEnum.h"
#include "B2DynItemIcon_ItemForge.generated.h"

/**
 * 
 */

enum class EItemForgeItemIconUsage
{
	EIFI_TargetItem,
	EIFI_InIvenItem
};

UCLASS()
class BLADEII_API UB2DynItemIcon_ItemForge : public UB2DynItemIcon
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<class UB2Button>				BTN_Area;

	

	TWeakObjectPtr<UImage>						MainIconImage;

	TWeakObjectPtr<UTextBlock>					ItemAmountText;

	TWeakObjectPtr<UTextBlock>					ItemLevelText;

	TWeakObjectPtr<UTextBlock>					ItemEnhanceLevelText;

	TWeakObjectPtr<UImage>						ItemLockedStateImage_Store;
	TWeakObjectPtr<UImage>						ItemLockedStateImage_Detail;

	TWeakObjectPtr<UImage>						ItemQualityBGImage;
	TWeakObjectPtr<UTextBlock>						ItemQualityTextBlock;

	TWeakObjectPtr<UTextBlock>					TB_Preset;
	TWeakObjectPtr<UImage>						IMG_EquippedCheck;
		
	TWeakObjectPtr<UImage>						DisabledSlotImage;
	
	TWeakObjectPtr<class UB2UIWidgetBase>		UIP_RedDot;
	
	TWeakObjectPtr<UTextBlock>					ItemNameText;

	

	TWeakObjectPtr<UPanelWidget>				P_EquipInfo;
	//TWeakObjectPtr<UImage>						IMG_PresetBG;
	TWeakObjectPtr<UImage>						BackgroundImage;
	TWeakObjectPtr<UImage>						IMG_CanNotSelect;
	TWeakObjectPtr<UOverlay>					OV_ItemQualitySet;
	TWeakObjectPtr<UOverlay>					OV_Selected;
	TWeakObjectPtr<UOverlay>					OV_LockedImages;

	TWeakObjectPtr<UCanvasPanel>	CP_Main;
	TWeakObjectPtr<UCanvasPanel>	CP_BackGroundOnly;

private:
	EItemForgeSlot								MaterialSlot;
	int32										FactoryID;
	bool										IsContainsPreset;
	class FLocalCharacterData*					CachedCharacterStore;

	EItemForgeItemIconUsage						ItemIconUseType;

	int32 requiredEnhanceLevel;
	int32 requiredItemLevel;
public:
	
	UB2DynItemIcon_ItemForge(const FObjectInitializer& ObjectInitializer);
	void SetUseType(EItemForgeItemIconUsage UseType) { ItemIconUseType = UseType; };
	virtual void CacheAssets() override;
	
	virtual void UpdateItemData(const FB2Item& InItem) override;
	void UpdateTargetItemData(int32 RefId,int32 Count);
	virtual void SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables) override;

	// Button input handling interfaces for ManualScrollBox feature.
	virtual void OnClickedSenderBTNArea() override;
	virtual void OnPressedSenderBTNArea() override;
	virtual void OnReleasedSenderBTNArea() override;

	void InitItemForgeSlot(EItemForgeSlot EIFS, EItemForgeItemIconUsage UseType, int32 FactoryId, int32 enhanceLevel = 0, int32 itemLevel=0);
	void SetMaterialSlotType(EItemForgeSlot EIFS) { MaterialSlot = EIFS; }

	void ShowJustEmptySlot();
	
	bool IsContainsCurrentPreset(const FB2Item& InItem);
	ESlateVisibility GetDisabledSlotImageVisibility() const;
	bool bIsNativeItemLocked;
	UFUNCTION()
		void OnClickBTNArea();
	UFUNCTION()
		void AskForUnlockItemPopupCommon();

	void SetOV_SelectedVisibility(ESlateVisibility VisibleType) { OV_Selected->SetVisibility(VisibleType); }

private:
	


	FText GetItemLevelText() const;
	FText GetItemAmountText() const;
	FText GetItemEnhanceLevelText() const;
	FText GetEquipPresetText() const;

	FText SetItemEnhanceLevelText(int32 enhanceLevel=0) const;
	FText SetItemLevelText(int32 itemLevel=1) const;
};
