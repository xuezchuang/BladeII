// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "B2UIGuildTeam.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildTeam : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIGuildTeam(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	
	void SetData(int32 InTeamIndex);
	void SetBattleState(EGuildTeamState GuildTeamState);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UOverlay>	X_O_WaitBattle;
	TWeakObjectPtr<UOverlay>	X_O_StartBattle;
	TWeakObjectPtr<UOverlay>	X_O_EndBattle;

	TArray<TWeakObjectPtr<UTextBlock>>	X_TB_TeamNums;
};
