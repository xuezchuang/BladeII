// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UILevelupAutoSelect.generated.h"



USTRUCT(BlueprintType)
struct FAutoSelectColorSet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FLinearColor EnableColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FLinearColor DisableColor;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UILevelupAutoSelect : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	UB2UILevelupAutoSelect(const FObjectInitializer& ObjectInitializer);

	bool IsEnableLeveupItem() const { return EnableLevelUpItem; }
	bool IsEnableEnhanceItem() const { return EnableEnhanceItem; }
	bool IsEnableEssenceItem() const { return EnableEssensceItem; }
	bool IsEnableAutoSelect(const FB2Item& IngredCandidate);

	bool IsMaxItemLevelUpExpectedByCurrentSelection();

	int32 GetPendingItemCount();

	UFUNCTION()
	void SetSelectState(bool IsEnabled1, bool IsEnabled2, bool IsEnabled3);
	void SetSelectedItemPart(class UB2LobbyInventory* InLobbyInventory);
	void SetSelectItems(int32 InGrade);
	void SetUnSelectItems(int32 InGrade);

	void SetVisibleLevelupButton(bool Enable);
	void SetVisibleEnhanceButton(bool Enable);
	void SetVisibleEssensceButton(bool Enable);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectEnabled(int32 StarGrade, bool IsEnabled);

private:
	void UpdateTargetItemInfo();
	void UpdateSelectedItem();
	void UpdateItemMapForGrade();
	void UpdateItemMapForPending();
	void UpdatePartsInfo();
	void UpdateItemLevel();

	UFUNCTION()
	void OnClicked_Close();

	UFUNCTION()
	void OnClicked_AutoLevelup();

	UFUNCTION()
	void OnClicked_Select();

	UFUNCTION()
	void OnClicked_Levelup();

	UFUNCTION()
	void OnClicked_Enhance();

	UFUNCTION()
	void OnClicked_Essence();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FAutoSelectColorSet TextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FAutoSelectColorSet StarColor;

private:
	TArray<TWeakObjectPtr<class UB2UIAutoSelectedItemParts>> SeletedParts;

	TMap<int32, TArray<FB2Item>> GradeItems;
	TMap<int32, TArray<FB2Item>> PendingItems;

	UB2LobbyInventory* LobbyInventory;

	bool EnableLevelUpItem;
	bool EnableEnhanceItem;
	bool EnableEssensceItem;

	int32 TargetItemLevel;

	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_AutoLevelUp;

	TWeakObjectPtr<UB2Button> BTN_Levelup;
	TWeakObjectPtr<UB2Button> BTN_Enhance;
	TWeakObjectPtr<UB2Button> BTN_EssenceItem;
	TWeakObjectPtr<UB2Button> BTN_Select;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Guide;
	
	TWeakObjectPtr<UImage> IMG_Levelup_Unselect;
	TWeakObjectPtr<UImage> IMG_Enhance_Unselect;
	TWeakObjectPtr<UImage> IMG_Essence_Unselect;
	TWeakObjectPtr<UImage> IMG_Levelup_Select;
	TWeakObjectPtr<UImage> IMG_Enhance_Select;
	TWeakObjectPtr<UImage> IMG_Essence_Select;

	TWeakObjectPtr<UImage> IMG_Levelup_Check;
	TWeakObjectPtr<UImage> IMG_Enhance_Check;
	TWeakObjectPtr<UImage> IMG_Essence_Check;

	TWeakObjectPtr<UTextBlock> TB_Levelup_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Enhance_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Essence_Unselect;
	TWeakObjectPtr<UTextBlock> TB_Levelup_Select;
	TWeakObjectPtr<UTextBlock> TB_Enhance_Select;
	TWeakObjectPtr<UTextBlock> TB_Essence_Select;
	
	TWeakObjectPtr<UTextBlock> TB_Static_ItemLevel;
	TWeakObjectPtr<UTextBlock> TB_CurrentItemLevel;
	TWeakObjectPtr<UTextBlock> TB_CurrentItemMaxLevel;
	TWeakObjectPtr<UTextBlock> TB_SelectComplete;

	TWeakObjectPtr<UTextBlock> TB_AutoLevelUp;
	TWeakObjectPtr<UTextBlock> TB_AutoLevelUpCost;

	TWeakObjectPtr<UPanelWidget> P_ItemLevel;

public:
	static const int32 MAX_AUTO_GRADE = 12;
	static const int32 MAX_ITEM_LEVEL = 30;
	static const int32 MAX_ITEM_COUNT = 20;

};
