// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIGuildSchedule.generated.h"

UCLASS()
class BLADEII_API UB2UIGuildScheduleSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetSchedule(b2network::B2mdGuildBattleScheduleInfoPtr InSchedule);
	void SetDay(int32 Inday);
	void SetGuildBattleStete(int32 InState);
	void SetTime(int32 InStartHour, int32 InStartMin, int32 InEndHour, int32 InEndMin);

protected:
	TWeakObjectPtr<UTextBlock>				TB_GuildState;
	TWeakObjectPtr<UTextBlock>				TB_Time;
	TWeakObjectPtr<UTextBlock>				TB_Day;

	TWeakObjectPtr<UTextBlock>				TB_SelectGuildState;
	TWeakObjectPtr<UTextBlock>				TB_SelectTime;
	TWeakObjectPtr<UTextBlock>				TB_SelectDay;

	TWeakObjectPtr<UPanelWidget>			OV_D1;
	TWeakObjectPtr<UPanelWidget>			OV_Ing;

};


UCLASS()
class BLADEII_API UB2UIGuildSchedule : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetSchedule(const TArray<b2network::B2mdGuildBattleScheduleInfoPtr>& InSchedule);

protected:
	TWeakObjectPtr<UHorizontalBox>				X_SlotBox;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildScheduleSlot>		GuildScheduleSlot;
};
