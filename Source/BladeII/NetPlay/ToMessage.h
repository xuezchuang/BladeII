#pragma once
#include "BladeIIPlayer.h"

class ABladeIINetPlayer;

class ToMessageHelper
{
public:
	static FString MoveMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString RotationMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString AttackMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString StopAttackMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString StartSkillMessage(ABladeIINetPlayer*, uint32, uint32, uint8, EAttackState, ESkillAnimType);
	static FString TagPlayerMessage(ABladeIINetPlayer*, uint32, uint32, uint8, ETagAttackMode, EPCClass);
	static FString GuardMessage(ABladeIINetPlayer*, uint32, uint32, uint8, uint32, uint32, uint8, uint8);
	static FString StopGuardMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString CounterAttackMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString UpdateTargetMessage(ABladeIINetPlayer*, uint32, uint32, uint8, uint32, float);
	static FString ComboEndMessage(ABladeIINetPlayer*, uint32, uint32, uint8, uint8, bool);
	static FString ResurrectMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString QteNextMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
	static FString SpawnEndMessage(ABladeIINetPlayer*, uint32, uint32, uint8);
};