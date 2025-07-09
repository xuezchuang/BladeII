// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "B2TargetSelector.h"
#include "B2SkillParameter.h"
#include "B2PassiveSkill.generated.h"


UCLASS()
class UB2PassiveSkill : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	class UB2SkillTrigger* SkillTrigger;

	UPROPERTY(Transient)
	class UB2SkillCommand* SkillCommand;

	UPROPERTY(Transient)
	class ABladeIICharacter* OwnerCharacter;
	
	EnumMaskValue<ETargetSelectType> TargetSelectType;
	int32 RestrictExcuteCount;

public:
	virtual void BeginDestroy() override;

	void InitSkill(class ABladeIICharacter* InOwnerCharacter, int32 InRestrictExcuteCount, EnumMaskValue<ETargetSelectType>&& InTargetSelectType, class UB2SkillTrigger * InSkillTrigger, class UB2SkillCommand * InSkillCommand, const FB2SkillParameterContainer& InSkillParamContainer);
	void StartSkill();

	ABladeIICharacter* GetOwner() const;
};
