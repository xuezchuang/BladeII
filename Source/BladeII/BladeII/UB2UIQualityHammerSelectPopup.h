// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "B2DynItemIcon_EnhanceIngred.h"
#include "UB2DynItemIcon_Hammer.h"
#include "UB2UIQualityHammerSelectPopup.generated.h"
/**
 *
 */

DECLARE_DELEGATE(UB2UIQualityHammerSelectPopup_ClickClose);

UCLASS()
class BLADEII_API UB2UIQualityHammerSelectPopup : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	static class AB2LobbyGameMode* HammerCachedLobbyGM;

private:
	void UpdateStaticWidgets();
	void UpdateDynamicWidgets();

	//class UB2DynItemIcon_LobbyInven* CreateItemIcon();
	class UB2DynItemIcon_Hammer* CreateItemIcon();
	//class UB2DynItemIcon_EnhanceIngred* CreateItemIcon();

protected:
	virtual void CloseWidgetDelegate() override;

	virtual void CacheAssets() override;

public:
	UB2UIQualityHammerSelectPopup_ClickClose ClickCloseDelegate;
	UB2UIQualityHammerSelectPopup(const FObjectInitializer& ObjectInitializer);

	void UpdateItemInfo(FB2Item& TargetItem);

	UFUNCTION()
	void OnClickBTN_Select();
	UFUNCTION()
	void OnClickBTN_Close();

	void DestroyItemIcons();
	FVector2D GetAllowedSingleItemIconSize();

private:
	TWeakObjectPtr<UCanvasPanel>			CP_List;
	TWeakObjectPtr<UB2ScrollBox>			SB_HammerIcons;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_ItemDesc;
	TWeakObjectPtr<UTextBlock> TB_QualityRange;
	TWeakObjectPtr<UTextBlock> TB_ExpectedQuality;

	TWeakObjectPtr<UB2Button> BTN_Select;
	TWeakObjectPtr<UTextBlock> TB_Select;

	TWeakObjectPtr<UB2Button> BTN_Close;


	int32 lowQuality;
	int32 highQuality;

protected:
	class FClientDataStore* CachedDataStore;

	UPROPERTY(Transient, BlueprintReadWrite)
	class UCanvasPanel* MainPanelNativeRef;
	UPROPERTY(Transient)
	TArray<class UB2DynItemIcon_Hammer*> CreatedItemIcons;
	//TArray<class UB2DynItemIcon_LobbyInven*> CreatedItemIcons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_Hammer> ItemIconWidgetClass;
	//TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;
	UPROPERTY(Transient)
	class UB2DynItemIcon_Hammer* CreatedItemIcon;
	//class UB2DynItemIcon_LobbyInven* CreatedItemIcon;

	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;
	UPROPERTY(Transient, BlueprintReadWrite)
	class UB2ScrollBox* ItemDisplayPanelNativeRef;
};
