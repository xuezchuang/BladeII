// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIBattlePause.generated.h"

/*
* BattlePause menu
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattlePause : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UIBattlePause(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetMod(EB2GameMode);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	//===================================================Click
	UFUNCTION()
	void OnClickBtnContinue();
	UFUNCTION()
	void OnClickBtnWorldMap();
	UFUNCTION()
	void OnClickBtnTagMatch();
	UFUNCTION()
	void OnClickBtnTeamMatch();
	UFUNCTION()
	void OnClickBtnCounterAttack();
	UFUNCTION()
	void OnClickBtnHeroTower();
	UFUNCTION()
	void OnClickBtnVillage();
	UFUNCTION()
	void OnClickBtnConfiguration();
	UFUNCTION()
	void OnClickBtnRaid();
	UFUNCTION()
	void OnClickBtnGuild();
	UFUNCTION()
	void OnClickBtnDimension();

private:
	TArray<std::shared_ptr<class issue>> Issues;

protected:
	virtual void CloseWidgetDelegate();

	TWeakObjectPtr<UButton>			BTN_Continue;
	TWeakObjectPtr<UButton>			BTN_Configuration;
	TWeakObjectPtr<UButton>			BTN_Village;
	TWeakObjectPtr<UButton>			BTN_WorldMap;
	TWeakObjectPtr<UButton>			BTN_TagMatch;
	TWeakObjectPtr<UButton>			BTN_TeamMatch;
	TWeakObjectPtr<UButton>			BTN_CounterAttack;
	TWeakObjectPtr<UButton>			BTN_HeroTower;
	TWeakObjectPtr<UButton>			BTN_Raid;
	TWeakObjectPtr<UButton>			BTN_Guild;
	TWeakObjectPtr<UButton>			BTN_Dimension;

	TWeakObjectPtr<UPanelWidget>	WorldMapButtonSet;
	TWeakObjectPtr<UPanelWidget>	TeamMatchButtonSet;
	TWeakObjectPtr<UPanelWidget>	TagMatchButtonSet;
	TWeakObjectPtr<UPanelWidget>	CounterAttackButtonSet;
	TWeakObjectPtr<UPanelWidget>	HeroTowerButtonSet;
	TWeakObjectPtr<UPanelWidget>	RaidButtonSet;
	TWeakObjectPtr<UPanelWidget>	GuildButtonSet;
	TWeakObjectPtr<UPanelWidget>	DimensionButtonSet;
};
