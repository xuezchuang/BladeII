// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIBattleSkill.h"
#include "B2UIBattleSkillUnity.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleSkillUnity : public UB2UIBattleSkill
{
	GENERATED_BODY()

public:

	UB2UIBattleSkillUnity(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void SetSkillPointStack(float MaxPoint, float CurPoint) override;

private:
	void UpdateSkillPointStackUI(float InDeltaTime);

public:
	UPROPERTY(EditAnywhere)
	float GageChangeAllowAmountBySecound;

private:
	float m_fMaxPointStack;
	float m_fCurPointStack;

	float m_fPointStackRateForUI;
};
