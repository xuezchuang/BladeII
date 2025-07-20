#pragma once

#include "BladeIIPlayer.h"

const int8		MY_TEAM		=	1;
const int8		OTHER_TEAM	=	2;

const int8		STAGE		=	1;
const int8		BACKSTAGE	=	2;

struct FPlayerEntityInfo
{
	EPCClass	Class;
	int8		Team;

	float		Attack;
	float		Defense;

	FVector		Position;
	FVector		Direction;
	float		Health;
};

struct ConditionValue
{
	float		Tendency;
	float		Tension;
};

struct PercentWeight 
{
	float		MoveWeight;
	float		AttackWeight;
	float		DefenseWeight;
};

struct FAIMemory
{
	ConditionValue					MyCondition;

	TArray<FPlayerEntityInfo>		MyTeam;
	TArray<FPlayerEntityInfo>		OtherTeam;
};
