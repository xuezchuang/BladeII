// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIEtherDetailPopup.generated.h"

/**
 * 
 */
enum class DetailPopupType
{
	Character = 0,
	Scroll,
	Max
};

struct FSetInfo
{
	TWeakObjectPtr<UTextBlock>	TB_Name;
	TWeakObjectPtr<UTextBlock>	TB_Value;
	TWeakObjectPtr<UPanelWidget> PW_Slot;
	
	void SetText(FString InNameKey, FString InOptionKey, float InValue);
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherDetailPopup : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	void SetPopupInfo(int64 ID, EPCClass SelectPCClass, DetailPopupType InType);
	void SetPCClass(EPCClass SelectPCClass);
	bool IsOpen();

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	UFUNCTION()
		void OnClickClose();
	virtual void Init() override;

	UPROPERTY(EditAnywhere)
		int MAXLEVEL = 9;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickSell();
	UFUNCTION()
		void OnClickDecomposition();
	UFUNCTION()
		void OnClickEnhance();
	UFUNCTION()
		void OnClickEquip();
	UFUNCTION()
		void OnClickTakeOff();
	UFUNCTION()
		void OnClickLock();

	void SetLockImage(bool InIsLock);
	void Decomposition();
	void UnLockEther(const FB2UnlockEtherPtr EtherPtr);
	void AskForUnlockItemPopupCommon(const FText& InAskingItemActText);

private:
	EPCClass												PCClass;
	int64													EtherID;
	int32													EtherPart;
	bool													IsLock;
	bool													CompareIsLock;
	
	TWeakObjectPtr<UB2Button>								X_BTN_Close;

	TWeakObjectPtr<UB2Button>								X_BTN_Sell;
	TWeakObjectPtr<UTextBlock>								X_TB_Sell;
	TWeakObjectPtr<UOverlay>								X_O_Sell;
	TWeakObjectPtr<UB2Button>								X_BTN_Decomposition;
	TWeakObjectPtr<UTextBlock>								X_TB_Decomposition;
	TWeakObjectPtr<UOverlay>								X_O_Decomposition;
	TWeakObjectPtr<UB2Button>								X_BTN_Enhance;
	TWeakObjectPtr<UTextBlock>								X_TB_Enhance;
	TWeakObjectPtr<UOverlay>								X_O_Enhance;
	TWeakObjectPtr<UB2Button>								X_BTN_EquipORTakeOff;
	TWeakObjectPtr<UTextBlock>								X_TB_EquipOrTakeOffBtn;
	
	TWeakObjectPtr<UB2RichTextBlock>						X_RTB_MainOptionName;
	TWeakObjectPtr<UB2RichTextBlock>						X_RTB_MainOptionValue;
	TWeakObjectPtr<class UB2UIEtherScrollSlot>				X_UIP_EtherSlot;
	TArray<TWeakObjectPtr<class UB2UIEtherOptionSlot>>		UIP_EtherOptions;
	TWeakObjectPtr<UImage>									X_ING_TierBG;
	TWeakObjectPtr<UTextBlock>								X_TB_EtherTierName;
	TWeakObjectPtr<UImage>									X_IMG_Lock;
	TWeakObjectPtr<UImage>									X_IMG_Unlock;
	TWeakObjectPtr<UB2Button>								X_BTN_Lock;
	TWeakObjectPtr<UTextBlock>								X_TB_SetName;
	TWeakObjectPtr<UTextBlock>								X_TB_SetInfo;
	TArray<FSetInfo>										UIP_EtherSetOptionInfos;

	uint32													DecompositionEtherEventID;
	uint32													LockEtherEventID;
	uint32													UnlockEtherEventID;

	DetailPopupType											PopupType;
};
