// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "B2SkillParameter.h"
#include "BladeIIDamageType.h"
#include "B2SkillCommand.generated.h"

UCLASS(Blueprintable)
class BLADEII_API UB2SkillCommand : public UObject
{
	GENERATED_BODY()

public:
	void InitCommand(class UB2PassiveSkill* InOwnerPassiveSkill, const FB2SkillParameterContainer& InSkillParamContainer);
	virtual void Excute(TArray<class ABladeIICharacter*>& ApplyCharacters) {};

protected:
	void SetSkillParameter(const FB2SkillParameterContainer& InSkillParamContainer);

protected:
	UPROPERTY(Transient)
	class UB2PassiveSkill* OwnerPassiveSkill;
};


UCLASS(Blueprintable)
class BLADEII_API UB2SkillCommand_AddBuff : public UB2SkillCommand
{
	GENERATED_BODY()

public:
	virtual void Excute(TArray<class ABladeIICharacter*>& ApplyCharacters) override;

protected:
	void AddBonusProperty(class ABladeIICharacter* ApplyCharacter, float& OutBonusDuration, float& OutBonusAmount);

protected:
	UPROPERTY(EditAnywhere)
	FB2SkillParameter ApplyDuration;

	UPROPERTY(EditAnywhere)
	FB2SkillParameter ApplyAmount;

	UPROPERTY(EditAnywhere)
	EBuffType BuffType;
};

//그레고르 전용 하드코딩
UCLASS(Blueprintable)
class BLADEII_API UB2SkillCommand_Transfusion : public UB2SkillCommand_AddBuff
{
	GENERATED_BODY()

public:
	virtual void Excute(TArray<class ABladeIICharacter*>& ApplyCharacters) override;
};
