// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UILobbySelectCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FCharacterSlot, EPCClass)

UCLASS()
class BLADEII_API UB2UILobbySelectCharacterSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void SetInfo(EPCClass HeroClass, int32 Lv, int32 CombatPower, int32 MaxLevel, float ExpPercent);
	void SetSelectedState(bool selected);
	bool GetIsSelected();
	void SetSlotDelegate(FCharacterSlot Indelegate);

protected:
	UFUNCTION()
	void OnClickBTN_Select();

private:
	TWeakObjectPtr<UB2Button>  BTN_Select;
	TWeakObjectPtr<UWidgetSwitcher> WS_ClassImage;

	TWeakObjectPtr<UTextBlock> STB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_Level;

	TWeakObjectPtr<UProgressBar> PB_Exp;
	TWeakObjectPtr<UProgressBar> PB_ExpMax;

	EPCClass						CurrentPCClass;
	bool							bIsSelected;
	FCharacterSlot					SlotDelegate;
};

UCLASS()
class BLADEII_API UB2UILobbySelectCharacter : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void SetCharSlotInfos();
	void SlotDelegateFunction(EPCClass InValue);

protected:
	UFUNCTION()
	void OnClickBTN_Back();

private:
	TArray<TWeakObjectPtr<class UB2UILobbySelectCharacterSlot>> CharSlots;

	TWeakObjectPtr<UB2Button>  BTN_Back;
	TWeakObjectPtr<UTextBlock> TB_SelectCharacter;

	EPCClass	SelectPCClass;
};
