#include "ToMessage.h"
#include "BladeII.h"


#include "Packet.h"
#include "PacketMaker.h"
#include "BladeIINetPlayer.h"
#include "CommonStruct.h"
#include "B2SkillRTManager.h"
#include "B2AnimInstance.h"

FString ToMessageHelper::MoveMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto velocity = Player->GetVelocity();
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, velocity);
	packet::PackingHelper(payload, rotation);

	float fCurrentSpeed = Player->GetAnimInstance()->GetCurrentSpeed();
	payload << fCurrentSpeed;

	Player->SetOldLVR(location, velocity, rotation);

	return packet::FinalizePacket(packet::MOVE, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::RotationMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, rotation);

	Player->SetOldRotation(rotation);

	return packet::FinalizePacket(packet::ROTATION, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::AttackMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	return packet::FinalizePacket(packet::ATTACK, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::StopAttackMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	return packet::FinalizePacket(packet::STOPATTACK, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::StartSkillMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option, EAttackState FinalAttackState, ESkillAnimType CurrentSkillAnimIdx)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;

	payload << FinalAttackState;
	payload << CurrentSkillAnimIdx;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	return packet::FinalizePacket(packet::STARTSKILL, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::TagPlayerMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option, ETagAttackMode TagAttackMode, EPCClass TagPlayerClass)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;

	payload << TagAttackMode;
	payload << TagPlayerClass;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	return packet::FinalizePacket(packet::TAGPLAYER, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::GuardMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option, uint32 SkillCancel, uint32 NeedUpdateTarget, uint8 LastGuardBreakType, uint8 AttackState)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	payload << SkillCancel;
	payload << NeedUpdateTarget;
	payload << LastGuardBreakType;
	payload << AttackState;

	return packet::FinalizePacket(packet::GUARD, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::StopGuardMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	return packet::FinalizePacket(packet::STOPGUARD, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::CounterAttackMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	return packet::FinalizePacket(packet::COUNTERATTACK, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::UpdateTargetMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option, uint32 Id, float TargetDist)
{
	packet::ByteStream	payload;
	payload << Id;
	payload << TargetDist;

	return packet::FinalizePacket(packet::UPDATETARGET, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::ComboEndMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option, uint8 InLastAttackState, bool IsAttackCanceled)
{
	packet::ByteStream	payload;
	payload << InLastAttackState;
	payload << IsAttackCanceled;

	return packet::FinalizePacket(packet::COMBOEND, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::ResurrectMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	auto location = Player->GetLocation();
	auto rotation = Player->GetActorRotation();

	packet::ByteStream	payload;

	// 좌표값 적재
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	// 부활시 버프도 함께 적재해준다.
	TMap<EResurrectBuffType, int32> AppliedResurrectBuff;
	Player->GetSkillRTManager()->GetAppliedResurrectionBuffValues(AppliedResurrectBuff);

	// 부활 정보 
	payload << static_cast<uint8>(AppliedResurrectBuff.Num());
	for (auto& ResurrectBuff : AppliedResurrectBuff)
		payload << static_cast<uint8>(ResurrectBuff.Key) << static_cast<uint8>(ResurrectBuff.Value);

	return packet::FinalizePacket(packet::RESURRECT, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::QteNextMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	packet::ByteStream	payload;
	
	return packet::FinalizePacket(packet::QTE_NEXT, SenderId, ReceiverId, Option, payload);
}

FString ToMessageHelper::SpawnEndMessage(ABladeIINetPlayer* Player, uint32 SenderId, uint32 ReceiverId, uint8 Option)
{
	packet::ByteStream	payload;

	return packet::FinalizePacket(packet::SPAWN_END, SenderId, ReceiverId, Option, payload);
}
