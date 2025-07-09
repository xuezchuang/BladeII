// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "B2SkillParameter.h"
#include "EventSubsystem.h"
#include "B2SkillTrigger.generated.h"

UCLASS(Blueprintable)
class BLADEII_API UB2SkillTrigger : public UObject
{
	GENERATED_BODY()

protected:
	virtual void RegistEvent() {};
	
	void SetSkillParameter(const FB2SkillParameterContainer& InSkillParamContainer);

public:
	void UnRegistEvent();
	void InitTrigger(class UB2PassiveSkill* InOwnerPassiveSkill, const FB2SkillParameterContainer& InSkillParamContainer);

	virtual void BeginDestroy() override;

	void OnTriggeringPassiveSkill();

protected:
	UPROPERTY(Transient)
	class UB2PassiveSkill* OwnerPassiveSkill;

	TArray<issue_ptr>	Issues;
};

UCLASS(Blueprintable)
class BLADEII_API UB2SkillTrigger_StartGame : public UB2SkillTrigger
{
	GENERATED_BODY()
protected:
	virtual void RegistEvent() override;
};

UCLASS(Blueprintable)
class BLADEII_API UB2SkillTrigger_LowHealth : public UB2SkillTrigger
{
	GENERATED_BODY()
protected:
	virtual void RegistEvent() override;

protected:
	UPROPERTY(EditAnywhere)
		FB2SkillParameter TriggerHealtRatio;
};