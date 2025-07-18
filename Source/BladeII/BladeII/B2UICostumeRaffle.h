// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2LobbyUserWidget.h"
#include "B2LobbyInventory.h"
#include "B2UIBackWidget.h"
#include "B2UISelectiveLotteryCostumeSlot.h"
#include "../UI/B2RichTextBlock.h"
#include "B2UICostumeRaffle.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UICostumeRaffle : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void StartFromRaffle(const FB2Item& ItemInfo);

	UFUNCTION()
		void OnClickButton();

	UFUNCTION()
		void OnClickCostume();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

protected:
	virtual void CloseWidgetDelegate() override;

private:
	void CloseMe();

private:
	TWeakObjectPtr<class UB2UICostumeRaffleOp> ResultPage;

	EPCClass ChadedCharacterClass;
};

UCLASS()
class BLADEII_API UB2UICostumeRaffleOp : public UB2LobbyUserWidget
{
	GENERATED_BODY()

public:
	UB2UICostumeRaffleOp(const FObjectInitializer& ObjectInitializer);
	virtual void DestroySelf() override;
	virtual void CacheAssets() override;
	
	void StartFromRaffle(const FB2Item& ItemInfo);
	void UpdateItemData(const FB2Item& InBeforeItem, const FB2Item& InAfterItem);

private:
	void UpdateStaticText();
	void UpdateItemOptions();
	void DestroyItemOptions();
	void UpdataItemSetEffect();
	void UpdateSubWidgets(const FB2Item& InBeforeItem, const FB2Item& InAfterItem);

public:
	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconPanel;

	TWeakObjectPtr<UB2RichTextBlock> TB_Receive;
	TWeakObjectPtr<UTextBlock> TB_ResultType;
	TWeakObjectPtr<UB2RichTextBlock> TB_Costume;

	TWeakObjectPtr<UTextBlock> TB_Combat;
	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_CombatPowerDelta;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPointType;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPoint;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPointDelta;
	TWeakObjectPtr<UTextBlock> TB_ConfirmBtn;

	TWeakObjectPtr<UButton> BTN_Confirm;
	TWeakObjectPtr<UButton> BTN_Receive;
	TWeakObjectPtr<UButton> BTN_Costume;

	TWeakObjectPtr<UVerticalBox> VB_OptionsDisplayPanel;
	TWeakObjectPtr<UVerticalBox> VB_SpecialInfoDisplayPanel;

	TWeakObjectPtr<UB2UISelectiveLotteryCostumeSlot> RaffleIcon;

	TWeakObjectPtr<UWidgetAnimation> Anim_ItemRaffle;

	FB2Item NativeItemData_Before;
	FB2Item NativeItemData_After;

	/** A part of OptionsListingPanel display. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
		TSubclassOf<class UB2DynText_Multiple> IntrinsicOptionTitleClass;
	UPROPERTY(Transient)
		class UB2DynText_Multiple* CreatedIntrinsicOptionTitle;

	/** A part of OptionsListingPanel display. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
		TSubclassOf<class UB2DynText_ItemDetailOption> IntrinsicOptionDisplayClass;
	UPROPERTY(Transient)
		TArray<class UB2DynText_ItemDetailOption*> CreatedIntrinsicOptionDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
		TSubclassOf<class UB2UICostumeItemDetailSetEffect> UICostumeItemDetailSetEffectClass;

protected:
	/** The font color implying that this item will increase character stat somewhat compared to what's equipped currently. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
		FSlateColor ItemStatIncColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
		FSlateColor ItemStatSameColor;

	/** The font color implying that this item will decrease (or just steady) character stat somewhat compared to what's equipped currently. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
		FSlateColor ItemStatDecColor;
};