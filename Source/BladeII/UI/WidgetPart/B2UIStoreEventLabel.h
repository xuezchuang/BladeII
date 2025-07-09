// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStoreEventLabel.generated.h"


UCLASS()
class BLADEII_API UB2UIStoreEventLabel : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetVisibleLabel(bool bIsVisible);
	void SetLobbyEventLabel(bool bIsVisible);
	void SetStoreEventLabel(int32 InMark);
	void SetFundsSale(int32 InDiscountRate);
	void SetFundsSaleForSummon();
	void SetCashBonus(int32 InIncreaseRate);
	void SetLimitLevel(bool bIsLimitLevel = false);

protected:

private:

	// Lobby
	TWeakObjectPtr<UOverlay> OV_Event_Lobby;
	TWeakObjectPtr<UTextBlock> TB_Event_Lobby;

	// Store, Summon Item
	TWeakObjectPtr<UOverlay> OV_Sale;
	TWeakObjectPtr<UTextBlock> TB_Sale;
	TWeakObjectPtr<UTextBlock> TB_SaleValue;

	TWeakObjectPtr<UOverlay> OV_Bonus;
	TWeakObjectPtr<UTextBlock> TB_Bonus;
	TWeakObjectPtr<UTextBlock> TB_BonusValue;
	
	TWeakObjectPtr<UOverlay> OV_Hit;
	TWeakObjectPtr<UTextBlock> TB_Hit;

	TWeakObjectPtr<UOverlay> OV_Hot;
	TWeakObjectPtr<UTextBlock> TB_Hot;

	TWeakObjectPtr<UOverlay> OV_New;
	TWeakObjectPtr<UTextBlock> TB_New;

	TWeakObjectPtr<UOverlay> OV_LimitLevel;
	TWeakObjectPtr<UTextBlock> TB_LimitLevel;

	TWeakObjectPtr<UOverlay> OV_SaleForSummon;
	TWeakObjectPtr<UTextBlock> TB_SaleForSummon;

	TWeakObjectPtr<UOverlay> OV_Limited;
	TWeakObjectPtr<UTextBlock> TB_Limited;
};
