#pragma once

#include "B2UIWidget.h"
#include "B2UISummonItemSlotItem.h"
#include "B2UIStoreMileage.h"
#include "B2UIStoreMileageRewardPopup.h"
#include "BladeIIPlayer.h"
#include "EventSubsystem.h"
#include "B2UISummonItem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISummonItem : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UISummonItem(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	void SubscribeEvent();
	void UnsubscribeEvent();

	void OpenSelectCharacterPopupWindow(int32 SlotHashKey, bool UseTenTicket);

public:

	void SetTab();

	static ESummonItemDifficulty GetCurrentStageDifficulty();

	bool IsFreeSummonableSlot(int32 SlotHashKey);
	void InventoryFullSimplePopup(EPCClass PCClass);

	void OnResponseRewardShopMileage(FB2ResponseRewardShopMileagePtr ResponseRewardShopMileage);
	void OnMileageChanged();

protected:
	
	UFUNCTION()
		void SelectedGladiator();
	UFUNCTION()
		void SelectedAssassin();
	UFUNCTION()
		void SelectedWizard();
	UFUNCTION()
		void SelectedFighter();
	UFUNCTION()
		void CloseSelectCharacterPopupWindow();
	UFUNCTION()
		void OnClickSelectConfirm();

	void UpdateSelectCharacterPopupWindow();
	void UpdateSelectCharacterPopupWindowTitle(const FText& TitleText);

	void SetCharSelectPopupButtonState(ESlateVisibility VisibleType);
	void SetCharSelectPopupButtonVisible();

	void SetButtonLineVisible(EPCClass InPCClass);

	void OpenConfirmMsgPopupWindow();

	void SetForcedBlockHeader(bool IsBlock);

	UFUNCTION()
		void CloseConfirmMsgPopupWindow();

	UFUNCTION()
		void SummonItem();

	UFUNCTION()
		void SetOpenRewardPopupBClass();
	UFUNCTION()
		void SetOpenRewardPopupAClass();
	UFUNCTION()
		void SetOpenRewardPopupSClass();

	void OpenRewardPopup(EUIStoreMileageGrade MileageGrade);

	UFUNCTION()
		void OpenMileageGuidePopup();
	UFUNCTION()
		void CloseMileageGuidePopup();

	UFUNCTION()
		void OnClickChance();

public:

	ESummonItemCost CostType;
	int32 InCost;
	int32 InCalculatedCost;
	ESummonItemTabMenu CurrentTabMenu;
	int32 SummonCount;
	EPCClass SelectedCharacterClass;

private:

	// Tab
	TArray<TWeakObjectPtr<UB2UISummonItemSlotItem>> SummonItemSlotItems;

	/* ==================================================
	Those below are from CharSelectPopup..
	*/
	TWeakObjectPtr<UTextBlock> TB_CharSelectTitle;

	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupClose;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupGladiator;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupAssassin;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupWizard;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupFighter;
	TWeakObjectPtr<UB2Button> BTN_CharSelectConfirm;

	TArray<TWeakObjectPtr<UOverlay>>	CharacterButtonOnArr;
	TArray<TWeakObjectPtr<UOverlay>>	CharacterButtonOffArr;

	// Per character..
	TArray<TWeakObjectPtr<UB2UIStoreProductCost>> UIP_CharSelectPopupCosts;

	/* ================================================== */

	// Mileage
	TWeakObjectPtr<UB2UIStoreMileage> UIP_StoreMileage;
	TWeakObjectPtr<UB2Button> BTN_MileageGuidePopupClose;
	TWeakObjectPtr<UB2Button> BTN_MileageGuide;
	
	// Chance
	TWeakObjectPtr<UB2Button> BTN_Chance;
	TWeakObjectPtr<UTextBlock> TB_Chance;

	// Doumi Girl
	TWeakObjectPtr<UOverlay> OV_DoumiGirl;
	TWeakObjectPtr<UTextBlock> TB_DoumiGirl;

protected:

	/**
	Dynamically loaded UI parts.
	*/
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIBackWidgetBase> UIP_CharSelectPopupClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIStoreMileageRewardPopup> UIP_MileageRewardPopupClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2UIRewardIcon> MRP_RewardIconTemplate; // Part of MileageRewardPopup.. not for dynamic load
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIBackWidgetBase> UIP_MileageGuidePopupClass;

	class UB2UIBackWidgetBase* CreatedUIP_CharSelectPopup;
	class UB2UIStoreMileageRewardPopup* CreatedUIP_MileageRewardPopup;
	class UB2UIBackWidgetBase* CreatedUIP_MileageGuidePopup;

	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_CharSelectPopupPanel;
	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_MileageRewardPopupPanel;
	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_MileageGuidePopupPanel;

	void ConditionalCreateCharSelectPopup();
	void ConditionalCreateMileageRewardPopup();
	void ConditionalCreateMileageGuidePopup();

	void DestroyCharSelectPopup();
	void DestroyMileageRewardPopup();
	void DestroyMileageGuidePopup();

private:
	TArray<std::shared_ptr<issue>> Issues;

	ESummonItemDifficulty CurrentDifficulty;

	FB2AddedItems CurrentSummonedItems;

	int32 SelectedSlotHashKey;
	bool bUsedTenLotteryTicket;

	FDelegateHandle MileageChangedHandler;
};

#undef TAB_MENU_SLOT

#define TAB_MENU_SLOT(MenuName, TabElem)	\
	GET_SLOT_BYNAME(UB2Button, TabElem.Button, BTN_##MenuName##Tab);						\
	GET_SLOT_BYNAME(UPanelWidget, TabElem.SelectedTap, P_##MenuName##Set_A);				\
	GET_SLOT_BYNAME(UPanelWidget, TabElem.UnSelectedTap, P_##MenuName##Set_D);			
