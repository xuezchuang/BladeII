// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIEnterBattleMain.generated.h"

UCLASS()
class BLADEII_API UB2UIEnterBattleMain : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	virtual void RegisterUIMarkForRedDot() final;

	virtual void OnOpen(bool RightNow = false);

	void ResponseGetMatchStatus(const FB2TagMatchStatus& msg);
	void ResponseGetTeamBattleStatus(const FB2TeamBattleStatus& msg);
	void ResponseGetAssaultBattleStatus(const FB2ResponseGetAssaultBattleStatusPtr& msgPtr);

	void UpdateBattleSlot();
	void UpdateBattleSlot_RewardInfo();

	UFUNCTION()
	void OnClick_SlotButton(ELobbyModSceneElem eGameMode);

	FText GetLockedTimeText();
protected:

	UFUNCTION()
		bool RedDotCondition_PVPMatch();
	UFUNCTION()
		bool RedDotCondition_TeamMatch();
	UFUNCTION()
		bool RedDotCondition_Control();

private:
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_PVPMatch;
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_TeamMatch;
	TWeakObjectPtr<class UB2UIEnterDungeonSlot> UIP_Control;

	uint32 DeliveryGetMatchStatusTicket;
	uint32 DeliveryGetTeamBattleStatusTicket;
	uint32 DeliveryGetAssaultBattleStatusTicket;
	uint32 DeliveryCheckRedDotTicket;
};
