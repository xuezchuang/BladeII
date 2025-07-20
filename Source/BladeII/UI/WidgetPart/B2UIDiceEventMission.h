// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventMission.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventMission : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitDiceEventMission(const b2network::B2mdDiceEventMission& DiceEventMission);
	
private:
	void InitMissionState();
	void UpdateMissionState();
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

private:
	TWeakObjectPtr<UTextBlock>			TB_MissionProgress;
	TWeakObjectPtr<UTextBlock>			TB_MissionPoint;

	TWeakObjectPtr<UWidgetSwitcher>		WS_MissionState;

private:
	b2network::B2mdDiceEventMission		CachedMissionMD;
	b2network::B2DiceEventMission		CachedMissinState;
};
