// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIEther.generated.h"

/**
 * 
 */


struct FCharacterTab
{
	FCharacterTab()
		: BTN_Tab(nullptr), IMG_OnImage(nullptr), IMG_OffImage(nullptr) {}

	TWeakObjectPtr<UButton>										BTN_Tab;
	TWeakObjectPtr<UImage>										IMG_OnImage;
	TWeakObjectPtr<UImage>										IMG_OffImage;
	TWeakObjectPtr<class UB2UIWidgetBase>						NewIcon;

	void SetTabVisible(bool IsSelect)
	{
		if (IsSelect)
		{
			if (IMG_OnImage.IsValid())
			{
				IMG_OnImage->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			if (IMG_OffImage.IsValid())
			{
				IMG_OffImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			if (IMG_OnImage.IsValid())
			{
				IMG_OnImage->SetVisibility(ESlateVisibility::Collapsed);
			}
			if (IMG_OffImage.IsValid())
			{
				IMG_OffImage->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	void SetNewIcon(bool bNew)
	{
		if (NewIcon.IsValid())
		{
			NewIcon->SetVisibility(bNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEther : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void GetScrollDetailPopup(const int64 EtherID);
	UFUNCTION()
		void GetEquipDetailPopup(const int64 EtherID);
	UFUNCTION()
		void RefreshEtherUI(bool IsEquip);
	UFUNCTION()
		void SetEtherScroll(const int32 SetName);

	TWeakObjectPtr<class UB2UIEtherDetailPopup>					X_UIP_ScrollDetailPopup;
	TWeakObjectPtr<class UB2UIEtherDetailPopup>					X_UIP_EquipDetailPopup;

	void SetCharacterInfo(EPCClass SelectTabType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII B2UIEther")
	void PlayUnmountEtherSound_BP();

protected:
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BindDelegates() override;
	virtual bool OnBackButtonProc() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	void RefreshCharacterEtherUI(bool IsEquip);
	void RefreshScrollEtherUI();
	void SetCurrentSetName(int32 SetName);
	void ResponseSellEther(const FB2SellEtherPtr EtherPtr);
	void ResponseDecompositionEther(const FB2DecompositionEtherPtr EtherPtr);
	void EnhanceEther(const FB2EnhanceEtherPtr EtherPtr);
	void SetInvenCount(int32 InCount);
	void InitEtherUIInfo();
	void SellEtherItem(const int64 EtherID, const int32 EtherPrice);
	void ResultAllSellEtherItem(const int32 EtherPrice);

	UFUNCTION()
		void OnClickGladiator();
	UFUNCTION()
		void OnClickAssassin();
	UFUNCTION()
		void OnClickWizard();
	UFUNCTION()
		void OnClickFighter();
	UFUNCTION()
		void OnClickAutoTakeOff();
	UFUNCTION()
		void OnClickAutoAllSell();
	UFUNCTION()
		void OnClickAutoDecomposition();
	UFUNCTION()
		void OnClickEtherFiltering();
	UFUNCTION()
		void OnClickExtend();
	
	TWeakObjectPtr<UB2Button>									X_BTN_EtherFiltering;
	TWeakObjectPtr<UTextBlock>									X_TB_EtherFiltering;
	TWeakObjectPtr<UB2Button>									X_BTN_AllSell;
	TWeakObjectPtr<UTextBlock>									X_TB_AllSell;
	TWeakObjectPtr<UB2Button>									X_BTN_AutoDecomposition;
	TWeakObjectPtr<UTextBlock>									X_TB_AutoDecomposition;
	TWeakObjectPtr<UB2Button>									X_BTN_AutoTakeOff;
	TWeakObjectPtr<UTextBlock>									X_TB_AutoTakeOff;
	TWeakObjectPtr<UTextBlock>									X_TB_CurrentCount;
	TWeakObjectPtr<UTextBlock>									X_TB_MaxCount;
	TWeakObjectPtr<UTextBlock>									X_TB_EmptyText;
	TWeakObjectPtr<UButton>										X_BTN_Extend;
	TWeakObjectPtr<UImage>										X_IMG_Plus;

	TWeakObjectPtr<class UB2LobbyUI_SellItemPopup>				X_UIP_EtherSellPopup;
	TWeakObjectPtr<class UB2UIEtherFiltering>					X_UIP_EtherFiltering;
	TWeakObjectPtr<class UB2UIEnhanceEther>						X_UIP_EnhancePopup;
	TWeakObjectPtr<class UB2UIEtherScroll>						X_UIP_EtherScroll;
	TWeakObjectPtr<class UB2UIEtherCharacterSlot>				X_UIP_EtherCharacterSlot;
	TWeakObjectPtr<class UB2UIEtherOption>						X_UIP_EtherOption;

	TMap<EPCClass, FCharacterTab>								CharacterTabs;
	int32														CurrentSetName;
	EPCClass													CurrentTabType;

	bool														IsSendModifyAetherNewFlag;
	TWeakObjectPtr<class UB2UIPEtherSetToolTip>					UIP_EtherSetToolTip;
	float														CachedScrollOffset;

private:
	TArray<std::shared_ptr<class issue>>						Issues;
};


