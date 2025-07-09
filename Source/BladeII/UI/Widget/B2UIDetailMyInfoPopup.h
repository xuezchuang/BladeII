// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIDetailMyInfoPopup.generated.h"

DECLARE_DELEGATE_OneParam(FCharacterSlot, EPCClass)

UCLASS()
class BLADEII_API UB2UIDetailMyInfoSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	UB2UIDetailMyInfoSlot(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void SetCharInfo(EPCClass HeroClass, int32 Lv, int32 CombatPower);
	void SetSelectedState(bool selected);
	bool GetIsSelected();
	void SetSlotDelegate(FCharacterSlot Indelegate);

	UFUNCTION()
	void OnClickSelectButton();
private:
	TWeakObjectPtr<UWidgetSwitcher> MainSwitcher;

	TWeakObjectPtr<UB2Button> BTN_Select;

	TWeakObjectPtr<UTextBlock> STB_BattleScore;
	TWeakObjectPtr<UTextBlock> TB_Level;
	TWeakObjectPtr<UTextBlock> TB_BattleScore;

	TWeakObjectPtr<UProgressBar> ExpProgressBar;
	TWeakObjectPtr<UProgressBar> ExpProgressBar_Max;

	EPCClass						CurrentPCClass;
	bool							bIsSelected;
	FCharacterSlot					SlotDelegate;
};

UCLASS()
class BLADEII_API UB2UIDetailMyInfoPopup : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()
public:
	UB2UIDetailMyInfoPopup(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	
	void SetUserGuild();
	void SetUserNickName();
	void SetCharInfos();
	void SlotDelegateFunction(EPCClass InValue);
protected:
	UFUNCTION()
	void OnClickBTN_ChangeChar();
	UFUNCTION()
	void OnClickBTN_Close();
private:
	TArray<TWeakObjectPtr<UB2UIDetailMyInfoSlot>>		HeroArr;
	TWeakObjectPtr<UB2UIDetailMyInfoSlot> UIP_Gladiator;
	TWeakObjectPtr<UB2UIDetailMyInfoSlot> UIP_Assassin;
	TWeakObjectPtr<UB2UIDetailMyInfoSlot> UIP_Wizard;
	TWeakObjectPtr<UB2UIDetailMyInfoSlot> UIP_Fighter;

	TWeakObjectPtr<UImage> IMG_DefaultMark;

	TWeakObjectPtr<UTextBlock> TB_UserNickName;
	TWeakObjectPtr<UTextBlock> TB_UserGuildName;
	TWeakObjectPtr<UTextBlock> STB_ChangeChar;
	
	TWeakObjectPtr<class UB2UIGuildMark> UIP_GuildMark;

	TWeakObjectPtr<UB2Button> BTN_ChangeChar;
	TWeakObjectPtr<UB2Button> BTN_Close;

	EPCClass	SelectPCClass;
};
