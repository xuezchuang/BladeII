// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIInGameBuffIcon.h"
#include "B2UITeamMatchBattleSlot.h"
#include "B2GuildGameMode.h"
#include "B2UIGuildMatchBattleSlot.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildMatchBattleSlot : public UB2UITeamMatchBattleSlot
{
	GENERATED_BODY()
	
protected:
	UB2UIGuildMatchBattleSlot(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SeMercenarytData(FGuildMercenaryInfo Info, bool InMyTeam);
	void ConnectPawn(class ABladeIIPlayer* InPlayer);
	void RefreshPawnStatus();

protected:
	virtual void CacheAssets() override;

	virtual void OnClickImplement() override;

	UFUNCTION()
	void OnChangeViewTarget(UB2UIDocBase* Sender, class ABladeIIPlayer* NewTarget, class ABladeIIPlayer* OldTarget);


protected:
	UPROPERTY()
	class ABladeIIPlayer* ConnectedPlayer;

	int32 MercenaryID;
	float CacheHealth;
	float CacheMaxHealth;
	float CacheShield;
};
