// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "CoreMinimal.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UISealEquipmentMain.generated.h"

/**
 *
 */


UENUM()
enum class ESealSlotSwitcher : uint8
{
	On,
	Off,
	Lock,
};

UCLASS()
class BLADEII_API UB2UISealEquipmentSlot : public UB2UIWidget
{

	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void UpdateStaticText() override;


	void SealSlotSetting(const FSealOption& InOption, EPCClass InPCClass);
	void SetSwitcherState(ESealSlotSwitcher InType);
	void SetSealOptionText(EItemOption InOption, float Value, EPCClass InPCClass);
	void SetButtonStyle();

	void PlaySealOpenAnimation();
	void PlaySealAnimation();
	float GetSealOpenEndAniTime();
	float GetSealEndAniTime();

public:
	TWeakObjectPtr<UB2Button>					BTN_SealSlot;
	TWeakObjectPtr<UTextBlock>					TB_SealOff;
	TWeakObjectPtr<UTextBlock>					TB_SealOption;

	TWeakObjectPtr<UWidgetSwitcher>				WS_SlotState;
	TWeakObjectPtr<UOverlay>					O_Choice;

	TWeakObjectPtr<UWidgetAnimation> SealOpenAni;
	TWeakObjectPtr<UWidgetAnimation> SealAni;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UISealSlot")
	int32 OptionSlotNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UISealSlot")
	FSlateColor TintColor;
};


UCLASS()
class BLADEII_API UB2UISealEquipmentMain : public UB2UIWidget, public IB2UIBackWidget
{

	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BindDelegates() override;

	virtual void CloseWidgetDelegate() override;

	virtual void OnOpen(bool RightNow = false) override;

	virtual void OnClose(bool RightNow = false) override;

	void SubscribeEvents();
	void UnSubscribeEvents();


	virtual void UpdateStaticText() override;

	void OpenSealItemMain(const FB2Item& TargetItem);
	void UpdateTargetItem(const FB2Item& TargetItem);
	void UpdateSlot();
	void UpdateSealOptionList(int32 InSlotNumber);
	void UpdateCost(int32 InSlotNumber);

	void ResponseSealOpenUIRefresh(const FB2Item& TargetItem);
	void ResponseSealUIRefresh(const FB2Item& TargetItem);
	void ResponseCostRefresh(int32 InGold, int32 Gem);
	void ResponsematerialRefresh(const TArray<b2network::B2ItemServerInfoPtr>& InUpdatedMaterialItems, const TArray<int64>& InDeletedConsumableIDs);

	bool CheckSealPopup();

	void UIRefresh();
	void OpenSealRefresh();

	void RewardIconCreate(int32 InTemplateId, int32 InAmount);
	void OnClickUpdate(int32 InSlotNumber);

	void ActionFunction(bool InAction);

	void CheckTutorialNextStep();

	UFUNCTION()
	void OnClickSlot1();
	UFUNCTION()
	void OnClickSlot2();
	UFUNCTION()
	void OnClickSlot3();
	UFUNCTION()
	void OnClickSlot4();

	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnClickUnLock();

	UFUNCTION()
	void OnClickSeal();

public:
	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> TargetItemIcon;

	//Close

	TWeakObjectPtr<UB2Button>						BTN_Close;

	/////////////////////////////////////////////////////////////////////
	TWeakObjectPtr<UVerticalBox>					VB_OptionList;
	TWeakObjectPtr<UHorizontalBox>					HB_SealMaterial;

	TWeakObjectPtr<UB2Button>						BTN_Seal;
	TWeakObjectPtr<UB2Button>						BTN_UnLock;

	TWeakObjectPtr<UB2Button>						BTN_NoAction;

	TWeakObjectPtr<class UB2UIStoreProductCost>			UIP_SealCost;
	TWeakObjectPtr<class UB2UIStoreProductCost>			UIP_UnLockCost;

	TWeakObjectPtr<UScrollBox>						SB_SealMaterial;

	TWeakObjectPtr<UCanvasPanel>					CP_Seal;
	TWeakObjectPtr<UCanvasPanel>					CP_Lock;

	//staticText

	TWeakObjectPtr<UTextBlock>						TB_Title;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_OptionTitle;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_OptionDesc;

	TWeakObjectPtr<UTextBlock>						TB_SealButton;

	TWeakObjectPtr<UTextBlock>						TB_LockDesc;
	TWeakObjectPtr<UTextBlock>						TB_UnLock;

	TArray<TWeakObjectPtr<UB2UISealEquipmentSlot>>		ArraySealSlot;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UISeal")
	TSubclassOf<class UB2DynText_Multiple>		UISealOptionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UISeal")
	TSubclassOf<class UB2UIRewardIcon>		UIRewardIconClass;


private:
	FB2Item CacheItem;
	int32 SelectSlotNumber;

	uint32 DeliveryOpenSealSlotItemTicket;
	uint32 DeliveryOpenSealSlotItemErrorTicket;
	uint32 DeliveryCarveSealItemTicket;
	uint32 DeliveryCarveSealItemErrorTicket;

	FTimerHandle DelayCallbackTH;
};
