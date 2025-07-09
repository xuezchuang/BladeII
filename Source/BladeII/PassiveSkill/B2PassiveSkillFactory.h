// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "B2PassiveSkill.h"
#include "B2SkillParameter.h"
#include "B2GuildNPCInfo.h"


UENUM(BlueprintType)
enum class SkillTriggerType : uint8
{
	NonTrigger = 0,
	StartGame,
	LowHPPercent,
};


namespace PassiveSkillFactory
{
	UB2PassiveSkill* CreateMasteryPassiveSkill(class ABladeIICharacter* SkillOwner, const FMercenaryMasterySkillInfo & Info, float ServerValue);

};