// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIManager_Lobby.h"
#include "B2UI_TotemInfoPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UI_TotemInfoPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	virtual void CloseWidgetDelegate() override;
	
public:
	void InitTotemInfoPopup(const FVector2D& v2PopupPosition, const float& fPopupSizeX, const FB2Totem& InTotem);

	class UPanelWidget* GetPopupBackGroundPanel();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();
	void UnsubscribeDecomposition();

	void UpdateTotemInfo(const FB2Totem& InTotem);
	void UpdateTotemInfoSlot(const FB2Totem& InTotem);
	void UpdateTotemInfoOptionList(const TArray<FB2TotemSubOptionDetail>& ArrayItemOption);
	void UpdateTotemSmeltingBonus(const FB2Totem& InTotem);
	void UpdateEquipOrReleaseBtn(const FB2Totem& InTotem);
	void UpdateWidgetVisibleState(const FB2Totem& InTotem);
	void UpdateCombatPower(const FB2Totem& InTotem, const FB2Totem* pEquippedTotemAtSamePlace);
	void UpdatePrimaryPoint(const FB2Totem& InTotem, const FB2Totem* pEquippedTotemAtSamePlace);
	void UpdateSmeltingLevel(const FB2Totem& InTotem);

	void OpenSmeltingPopup();

	void OnDelaySmeltingCallback();
	void OnDelayDecompositionCallback();
	void OnDelaySellCallback();

	void SetDeltaText(TWeakObjectPtr<UB2RichTextBlock> pRTB, const int32& iDeltaValue, const bool& IsVisible);

	FDJLegacySubpopupAddPanelInfo GetDJLegacySubpopupInfo();

private:
	void RequestEquipTotem();
	void RequestUnEquipTotem();

	void ResponseEquipTotems(const FB2EquipTotemsPtr& EquipTotemsPtr);
	void ResponseUnequipTotems(const FB2UnequipTotemsPtr& UnequipTotemsPtr);
	void Sell();
	void Decomposition();

	FText GetSubOptionTypeText(ETotemSubOptionType InSubOptionType);

private:
	void NoticeErrorPopupCommon(const FText& InAskingText);
	void AskForUnlockItemPopupCommon(const FText& InAskingItemActText);

private:
	UFUNCTION()
		void OnClickBTN_Close();

	UFUNCTION()
		void OnClickBTN_Smelting();

	UFUNCTION()
		void OnClickBTN_Decomposition();

	UFUNCTION()
		void OnClickBTN_Sell();

	UFUNCTION()
		void OnClickBTN_EquipOrRelease();

	UFUNCTION()
		void OnClickBTN_ItemLock();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void PlayEquipOrReleaseSound(bool bIsForEquip);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void PlayLockOrUnlockSound(bool bIsforLock);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIHeaderBezel")
		float fTopBezel_BP;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PopupSubOptionType")
		TArray<ETotemSubOptionType> SubOptionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OptionColor")
		FSlateColor OptionChanceColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OptionColor")
		FSlateColor OptionValueColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
		FLinearColor ItemStatIncColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
		FLinearColor ItemStatSameColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
		FLinearColor ItemStatDecColor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UITotemOption")
		TSubclassOf<class UB2DynText_Multiple>		UITotemOptionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UITotemSmeltingBonus")
		TSubclassOf<class UB2UI_TotemSmeltingBonus>	UITotemSmeltingBonusClass;

private:
	TWeakObjectPtr<UCanvasPanel>		CP_PopupBackground;

	TWeakObjectPtr<UCanvasPanel>		CP_PopupMain;	// 위치 잡을때 사용

	TWeakObjectPtr<UB2Button>			BTN_Close;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_Combat;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_CombatPower;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_CombatPowerDelta;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_PrimaryPointType;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PrimaryPoint;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PrimaryPointDelta;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_SmeltingSuccess;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_SmeltingLevel;

	TWeakObjectPtr<UB2Button>			BTN_Smelting;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Smelting;

	TWeakObjectPtr<UB2Button>			BTN_Decomposition;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Decomposition;

	TWeakObjectPtr<UB2Button>			BTN_Sell;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Sell;

	TWeakObjectPtr<UB2Button>			BTN_EquipOrRelease;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_EquipOrRelease;

	TWeakObjectPtr<UB2Button>			BTN_ItemLock;
	TWeakObjectPtr<UImage>				IMG_LockButtonLocked;
	TWeakObjectPtr<UImage>				IMG_LockButtonUnLocked;

	TWeakObjectPtr<class UB2LobbyUI_TotemInventorySlot>	UIP_TotemInventorySlot;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_OptionTitle;
	TWeakObjectPtr<UVerticalBox>		VB_OptionList;

	TWeakObjectPtr<UVerticalBox>		VB_OptionsDisplayPanel;
	TWeakObjectPtr<UVerticalBox>		VB_SmeltingBonus;

private:
	TArray<issue_ptr> Issues;

	FTimerHandle DelayCallbackTH;
		
	FB2Totem CachedB2TotemInfo;
	EPCClass CachedPCClass;

	uint32 DecompositionEquipTotemEventID;
};
