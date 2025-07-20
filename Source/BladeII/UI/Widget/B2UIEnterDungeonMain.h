// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIEnterDungeonMain.generated.h"


UCLASS()
class BLADEII_API UB2UIEnterDungeonMain : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

protected:

	bool RedDotCounterDungeon();
	bool RedDotHeroTower();
	bool RedDotRaidDungeon();
	bool RedDotDimension();

public:
	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void RegisterUIMarkForRedDot() final;

	virtual void OnOpen(bool RightNow = false);

	void ResponseGetHeroTower(const FB2ResponseGetHeroTowerPtr& msgPtr);
	void ResponseGetCounter(const FB2GetCounterDungeon& msg);
	void ResponseGetRaid(const FB2ResponseGetRaidPtr& msgPtr);
	void ResponseGetDimension(const FB2ResponseGetDimensionPtr& msgPtr);

	void UpdateDungeonSlot();
	void UpdateDungeonSlot_RewardInfo();

	UFUNCTION()
	void OnClick_DungeonButton(ELobbyModSceneElem eGameMode);

	FText GetLockedTimeText();

private:
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_Counter;
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_HeroTower;
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_Raid;
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_Dimension;

	uint32 DeliveryGetHeroTowerTicket;
	uint32 DeliveryGetCounterTicket;
	uint32 DeliveryGetRaidTicket;
	uint32 DeliveryCheckRedDotTicket;
	uint32 DeliveryGetDimensionTicket;

};
