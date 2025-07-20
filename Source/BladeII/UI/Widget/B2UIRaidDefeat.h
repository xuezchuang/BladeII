// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIRaidDefeat.generated.h"

DECLARE_DELEGATE(FDelegateFinishDefeatCountDown);

UCLASS()
class BLADEII_API UB2UIRaidDefeat : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

protected:
	void UpdateContentText(int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance);
	bool IsResurrectPossibleByCost();
	void UpdateOnResurrectCost(int32 InValue);
	void SetOnResurrectEnableByRemainingChance(bool InBool);
	void SetFailCountDown(int32 LimitTime);
	void SetVisibleFailCountOnly();
	void ResurrectImpossibleByCostAction();
	bool IsOnlyVisibleFailCount();
	void SetContentText(bool bTrueForUpper, const FText& InText);
	void UpdateBuffIconContent();
	void UpdateResurrctBtnEnableState();

	void SetEnableRessurectButton(bool bEnable);

	UFUNCTION()
	void OnClickBTN_Quit();
	UFUNCTION()
	void OnClickBTN_Resurrect();
	UFUNCTION()
	void OnBuffIconPartClicked(EResurrectBuffType ClickedBuffType);
	//====================================================Doc Delegate
	UFUNCTION()
	void OnChangeRemainingResurrectionChance(class UB2UIDocBase* Sender, int32 RemainingResurrectionChance, int32 PrevRemainingResurrectionChance);
	UFUNCTION()
	void OnChangeResurrectionCost(class UB2UIDocBase* Sender, int32 ResurrectionCost, int32 PrevResurrectionCost);
	UFUNCTION()
	void OnChangedFailCountDown(class UB2UIDocBase* Sender, int32 diff, int32 PrevDiff);

private:
	EResurrectBuffType SelectedBuffType;
	FDelegateFinishDefeatCountDown		FFinishDefeatCountDown;

	TWeakObjectPtr<UPanelWidget>		P_FailCountDown;
	TWeakObjectPtr<UPanelWidget>		P_NoResurrectMsg;
	TWeakObjectPtr<UPanelWidget>		P_RessurectButton;
	TWeakObjectPtr<UPanelWidget>		P_Buff;
	
	TWeakObjectPtr<UTextBlock>			TB_CountDownNumber;
	TWeakObjectPtr<UTextBlock>			TB_ResurrectCost;
	TWeakObjectPtr<UTextBlock>			TB_Content_Upper;

	TWeakObjectPtr<UTextBlock>			STB_Title;
	TWeakObjectPtr<UTextBlock>			STB_BtnQuit;
	TWeakObjectPtr<UTextBlock>			STB_BtnResurrect;
	TWeakObjectPtr<UTextBlock>			STB_Fail;
	TWeakObjectPtr<UTextBlock>			STB_NoGem;
	TWeakObjectPtr<UTextBlock>			STB_LimitDesc1;
	TWeakObjectPtr<UTextBlock>			STB_LimitDesc2;

	TWeakObjectPtr<UB2Button>			BTN_Quit;
	TWeakObjectPtr<UB2Button>			BTN_Resurrect;

	TWeakObjectPtr<UImage>				IMG_Jewel;

	TWeakObjectPtr<class UB2UIResurrectBuffIcon> UIP_BuffIcon_Attack;
	TWeakObjectPtr<class UB2UIResurrectBuffIcon> UIP_BuffIcon_Defense;
	TWeakObjectPtr<class UB2UIResurrectBuffIcon> UIP_BuffIcon_Health;
};
