// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIResurrectBuffIcon.h"
#include "EventSubsystem.h"
#include "B2UIDefeat.generated.h"

DECLARE_DELEGATE(FDelegateFinishDefeatCountDown);

/*
* In-game post player dead resurrection menu.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDefeat : public UB2UIWidget, public IB2UIBackWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

		EResurrectBuffType SelectedBuffType;

public:
	UB2UIDefeat(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	bool IsResurrectPossibleByCost();
	void UpdateOnResurrectCost(int32 InValue);
	// Re-organize some widgets according to this stuff.
	void SetOnResurrectEnableByRemainingChance(bool InBool);

	void SetFailCountDown(int32 LimitTime);
	void SetVisibleFailCountOnly();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void ImpossibleResurrectChance_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void ImpossibleResurrectCost_BP();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void UpdateStaticTextByGameMode();
	void UpdateContentText(int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance);

	void ResurrectImpossibleByCostAction();
	bool CanChargeGemByShop();
	bool IsOnlyVisibleFailCountGameMode();

	// True for upper, false for center. The other one will be hidden
	void SetContentText(bool bTrueForUpper, const FText& InText);

	void UpdateBuffIconContent();
	void UpdateResurrctBtnEnableState();

	void SetEnableRessurectButton(bool bEnable);
	void SetVisibleReaminChanceContent(bool bVisible);

	void ResponseSetSummonItemUIData(const FB2ResponseGetLotteryShopPtr& ShopDataPtr);
	void GotoSummonItemStore();

	//===================================================Click
	UFUNCTION()
	void OnClickBtnQuit();
	UFUNCTION()
	void OnClickBtnResurrect();
	UFUNCTION()
	void OnClickBtnConfirm();

	/** Click event from ResurrectBuffIcon */
	UFUNCTION()
	void OnBuffIconPartClicked(EResurrectBuffType ClickedBuffType);

	//====================================================Doc Delegate
	UFUNCTION()
	void OnChangeRemainingResurrectionChance(class UB2UIDocBase* Sender, int32 RemainingResurrectionChance, int32 PrevRemainingResurrectionChance);
	UFUNCTION()
	void OnChangeResurrectionCost(class UB2UIDocBase* Sender, int32 ResurrectionCost, int32 PrevResurrectionCost);

	UFUNCTION()
	void OnChangedFailCountDown(class UB2UIDocBase* Sender, int32 diff, int32 PrevDiff);
	
	UFUNCTION()
	void OnClickBtnItemEnhance();

	UFUNCTION()
	void OnClickBtnSkillEnhance();

	UFUNCTION()
	void OnClickBtnItemPurchase();
public:
	FDelegateFinishDefeatCountDown	FFinishDefeatCountDown;

protected:
	virtual void CloseWidgetDelegate() override;

	TWeakObjectPtr<UOverlay>					O_Main;
	TWeakObjectPtr<UButton>						BTN_Quit;
	TWeakObjectPtr<UTextBlock>					TB_BtnQuit;

	TWeakObjectPtr<UButton>						BTN_Confirm;
	TWeakObjectPtr<UTextBlock>					TB_BtnConfirm;

	TWeakObjectPtr<UButton>						BTN_Resurrect;
	TWeakObjectPtr<UTextBlock>					TB_BtnResurrect;
	TWeakObjectPtr<UTextBlock>					TB_ResurrectCost;

	TWeakObjectPtr<UTextBlock>					TB_Title;
	TWeakObjectPtr<UTextBlock>					TB_Content_Upper;
	TWeakObjectPtr<UImage>						IMG_UpperTextBG;

	TWeakObjectPtr<UB2UIResurrectBuffIcon>		UIP_BuffIcon_Attack;
	TWeakObjectPtr<UB2UIResurrectBuffIcon>		UIP_BuffIcon_Defense;
	TWeakObjectPtr<UB2UIResurrectBuffIcon>		UIP_BuffIcon_Health;

	TWeakObjectPtr<UOverlay>					O_FailCountDown;
	TWeakObjectPtr<UTextBlock>					TB_CountDownNumber;

	TWeakObjectPtr<UPanelWidget>				P_RessurectButton;
	TWeakObjectPtr<UImage>						IMG_Jewel;
	TWeakObjectPtr<UTextBlock>					STB_NoGem;

	TWeakObjectPtr<UPanelWidget>				P_Content;
	TWeakObjectPtr<UPanelWidget>				P_FailContent;
	
	TWeakObjectPtr<UTextBlock>					TB_Arin;
	TWeakObjectPtr<UTextBlock>					TB_ConsultMessage;

	TWeakObjectPtr<UB2Button>					BTN_ItemEnhance;
	TWeakObjectPtr<UTextBlock>					TB_ItemEnhance;

	TWeakObjectPtr<UB2Button>					BTN_SkillEnhance;
	TWeakObjectPtr<UTextBlock>					TB_SkillEnhance;

	TWeakObjectPtr<UB2Button>					BTN_ItemPurchase;
	TWeakObjectPtr<UTextBlock>					TB_ItemPurchase;

private:
	TArray<std::shared_ptr<issue>>				Issues;

};
