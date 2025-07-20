// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2UIEventSlotDice.generated.h"

UCLASS()
class BLADEII_API UB2UIEventSlotDice : public UB2UIEventSlot
{
	GENERATED_BODY()

public:
	void InitEventSlotDiceCell(const b2network::B2mdDiceEventCell& DiceEventCell);
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

protected:
	virtual void UpdateVisibleState() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIEventSlotDice")
	void StartAnimMovePosition_BP();

	UFUNCTION(BlueprintCallable, Category = "B2UIEventSlotDice")
	void EndAnimMovePosition_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIEventSlotDice")
	void StartAnimArrive_BP();

	UFUNCTION(BlueprintCallable, Category = "B2UIEventSlotDice")
	void EndAnimArrive_BP();

	void UpdateIMG_RewardIcon();
	void UpdateArriveState(int32 iPosition);

public:
	void SetArriveState(bool IsArrive);
	void SetVisibleStartIcon(const bool Visible);
	int32 GetCachedCellType() { return CachedDiceEventCellMD.cell_type; };

public:
	int32 GetCellNumberFromWidgetName();

private:
	TWeakObjectPtr<UWidgetSwitcher>				WS_Board;

	TWeakObjectPtr<UOverlay>					O_Arrive;

	//ws_reward
	TWeakObjectPtr<UTextBlock>					TB_RewardCount;

	//ws_times
	TWeakObjectPtr<UTextBlock>					TB_Times;

	//ws_start
	TWeakObjectPtr<UTextBlock>					TB_Start;

	//o_rewardIcon
	TWeakObjectPtr<UOverlay>					O_RewardIcon;
	TWeakObjectPtr<UImage>						IMG_Start;
	TWeakObjectPtr<UB2Image>					IMG_RewardIcon;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_RewardIcon;

private:
	int32										CellNumberFromWidgetName = -1;
	b2network::B2mdDiceEventCell				CachedDiceEventCellMD;
};
