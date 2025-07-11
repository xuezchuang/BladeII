// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
//#include "CoreMinimal.h"
//#include "UI/B2UIWidget.h"
//#include "B2LobbyUserWidget.h"
//#include "B2UIBackWidget.h"
//#include "B2LobbyUI_TotemInventorySlot.h"
//#include "B2TotemData.h"
//#include "B2UITotemRaffle.generated.h"
//
///**
// * 
// */
//UCLASS()
//class BLADEII_API UB2UITotemRaffle : public UB2UIWidget, public IB2UIBackWidget
//{
//	GENERATED_BODY()
//
//public:
//	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
//
//	virtual void Init() override;
//	void StartFromRaffle(const FB2Totem& InTotem);
//
//	UFUNCTION()
//		void OnClickButton();
//
//	UFUNCTION()
//		void OnClickTotem();
//
//protected:
//	virtual void CacheAssets() override;
//	virtual void BindDelegates() override;
//
//protected:
//	virtual void CloseWidgetDelegate() override;
//
//private:
//	void CloseMe();
//
//private:
//	TWeakObjectPtr<class UB2UITotemRaffleOp> UID_TotemRaffleOp;
//};
//
//UCLASS()
//class BLADEII_API UB2UITotemRaffleOp : public UB2LobbyUserWidget
//{
//	GENERATED_BODY()
//
//public:
//	UB2UITotemRaffleOp(const FObjectInitializer& ObjectInitializer);
//	virtual void DestroySelf() override;
//	virtual void CacheAssets() override;
//	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
//
//	void StartFromRaffle(const FB2Totem& InTotem);
//	void UpdateItemData(const FB2Totem& InBeforTotem, const FB2Totem& InAfterTotem);
//
//private:
//	void UpdateStaticText();
//	void UpdateItemOptions(const FB2Totem& InTotem);
//	void DestroyItemOptions();
//	void UpdataItemSetEffect();
//	void UpdateSubWidgets(const FB2Totem& InBeforTotem, const FB2Totem& InAfterTotem);
//	void UpdateTotemSmeltingBonus(const FB2Totem& InTotem);
//
//	FText GetSubOptionTypeText(ETotemSubOptionType InSubOptionType);
//public:
//	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconPanel;
//
//	TWeakObjectPtr<UB2RichTextBlock> TB_Receive;
//	TWeakObjectPtr<UTextBlock> TB_ResultType;
//	TWeakObjectPtr<UB2RichTextBlock> TB_Totem;
//
//	TWeakObjectPtr<UTextBlock> TB_Combat;
//	TWeakObjectPtr<UTextBlock> TB_CombatPower;
//	TWeakObjectPtr<UTextBlock> TB_CombatPowerDelta;
//	TWeakObjectPtr<UTextBlock> TB_PrimaryPointType;
//	TWeakObjectPtr<UTextBlock> TB_PrimaryPoint;
//	TWeakObjectPtr<UTextBlock> TB_PrimaryPointDelta;
//	TWeakObjectPtr<UTextBlock> TB_SmeltingSuccess;
//	TWeakObjectPtr<UTextBlock> TB_SmeltingLevel;
//	TWeakObjectPtr<UTextBlock> TB_ConfirmBtn;
//
//	TWeakObjectPtr<UButton> BTN_Confirm;
//	TWeakObjectPtr<UButton> BTN_Receive;
//	TWeakObjectPtr<UButton> BTN_Totem;
//
//	TWeakObjectPtr<UVerticalBox>	VB_OptionList;
//
//	//TWeakObjectPtr<UVerticalBox> VB_OptionsDisplayPanel;
//	TWeakObjectPtr<UVerticalBox> VB_SmeltingBonus;
//
//	TWeakObjectPtr<UB2LobbyUI_TotemInventorySlot> RaffleIcon;
//
//	TWeakObjectPtr<UWidgetAnimation> Anim_ItemRaffle;
//
//	FB2Totem NativeItemData_Before;
//	FB2Totem NativeItemData_After;
//
//	/** A part of OptionsListingPanel display. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
//		TSubclassOf<class UB2DynText_Multiple> UITotemOptionClass;
//
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UITotemSmeltingBonus")
//		TSubclassOf<class UB2UI_TotemSmeltingBonus>	UITotemSmeltingBonusClass;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PopupSubOptionType")
//		TArray<ETotemSubOptionType> SubOptionType;
//	
//
//
//protected:
//	/** The font color implying that this item will increase character stat somewhat compared to what's equipped currently. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
//	FSlateColor ItemStatIncColor;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
//	FSlateColor ItemStatSameColor;
//
//	/** The font color implying that this item will decrease (or just steady) character stat somewhat compared to what's equipped currently. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
//	FSlateColor ItemStatDecColor;
//};