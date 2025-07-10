// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "B2NoAnimSkillInfo.h"

UB2NoAnimSkillInfo::UB2NoAnimSkillInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ThunderstormWaitTime = 0.5f;
	ThunderstormDamage = 10.0f;
	ThunderstormFreezeDuration = 0.0f;
	ThunderstormImpulseScale = 1.0f;

	LimitationBuffType = EBuffType::Debuff_Immobilize;
	LimitationDuration = 10.0f;

	FirewallDamage = 10.0f;
	FirewallRadius = 1000.0;
	FirewallFreezeDuration = 0.0f;
	FirewallImpulseScale = 1.0f;
}
