// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "BladeIINetPlayer.h"

#include "BladeIIGameMode.h"
//#include "BehaviorTree/BehaviorTree.h"
//#include "ChannelManager.h"
#include "Serializer.h"
#include "Event.h"
#include "Packet.h"
#include "PacketMaker.h"
//#include "B2SkillRTManager.h"
#include "BladeIIPlayerAIController.h"
#include "B2NetGameMode.h"
#include "BladeIIUtil.h"
#include "B2PVPGameMode.h"
#include "B2RaidGameMode.h"
#include "BladeIINetCharacter.h"
//#include "B2UIManager_InGameCombat.h"
#include "B2ResurrectionZone.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
//#include "ToMessage.h"
#include "B2EtherContainer.h"
#include "B2EtherSetEffect.h"
#include "B2BuffModeEffectInfo.h"
//#include "B2AnimInstance.h"
//#include "B2CodeTable.h"
#include "B2GameDefine.h"
#include "FakeConfigure.h"

ABladeIINetPlayer::ABladeIINetPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	NetId		= 0;
	SetNetStatus(NET_MASTER);
	
	bEventsSubscribed		= false;
	GotRemoteDyingSignal	= false;
	Isolated				= false;

	OldLocation				= FVector::ZeroVector;
	OldVelocity				= FVector::ZeroVector;
	OldRotation				= FRotator::ZeroRotator;

	IsInvalidateInput		= false;
	bRootMotionBegun		= false;

	OldTargetActor			= nullptr;
	ResurrectionZoneActor	= nullptr;
	ResurrectionCount		= 0;

	NetTimer				= 0.0f;
	NetFrequency			= 0.05f;
	NetMessageLOD			= 0;
	UpdateLocationRadius	= 200.0f;

	bTemporarilyNetOff		= false;
	OldDamagedNum			= 0;

	EnableTakeDamageForNet  = true;

	LastResurrectLocation = FVector::ZeroVector;

	bSentGuardPacket		= false;

	m_nLastSendAnimStateIndex = -1;

	m_SkillAnimTypeForRemotePlayer = ESkillAnimType::ESA_End;

	SyncedAnimInfos.Empty();
}

void ABladeIINetPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//AB2NetGameMode* pGM = GetNetGameMode();
	//if (pGM)
	//{
	//	TArray<FNetSyncAnimInfo> AnimStateIndexInfos = pGM->GetSyncedAnimStateIndexInfo(NetId);
	//	UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::BeginPlay, Name =%s, Netid = %d, Class = %d, AnimStateIndexInfosNum = %d"), 
	//		*GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), AnimStateIndexInfos.Num());
	//	for (int i = 0; i < AnimStateIndexInfos.Num(); i++)
	//	{
	//		//패킷은 순차 적으로 쌓인다.
	//		SetSyncedAnimInfo(AnimStateIndexInfos[i]);
	//	}
	//}

	//FormalSetPointLightOn(false);  // 어차피 IsHeadPointLightAllowed() 가 false 여야..

	//SubscribeEvents();

	//if (GetNetStatus() == NET_SLAVE)
	//{
	//	Role = ROLE_SimulatedProxy;
	//}

	//m_vCachedLocationForRootMotionBlock = GetActorLocation();
}

void ABladeIINetPlayer::BeginDestroy()
{
	UnsubscribeEvents();

	DestroyResurrectionZoneActor();

	Super::BeginDestroy();
}

void ABladeIINetPlayer::Destroyed()
{
	UnsubscribeEvents();
	UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::Destroyed, Name = %s, Netid = %d, Class = %d"), *GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()));
	DestroyResurrectionZoneActor();

	Super::Destroyed();
}

void ABladeIINetPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
//	
// 	//CheckSendMove();
//
//	AdjustRootMotionBlock();
//
//	if (GetLocalRole() == ROLE_SimulatedProxy && HasRootMotion())
//	{
//		UpdateVelocity(FVector::ZeroVector);
//	}
//	
//#if !UE_BUILD_SHIPPING
//	NetMessageLOD	= NetFakeConfigure::GetInstance().GetNetMessageLOD();
//	NetFrequency	= NetFakeConfigure::GetInstance().GetNetMessageFrequency();
//#endif
//	
//	NetTimer	   += DeltaSeconds;
//
//	if (NetTimer >= NetFrequency && !bTemporarilyNetOff)
//	{
//		NetTimer = 0.0f;
//		NetSendUpdateLocation();
//	}
//	
//	// Ai마스터 게임모드에 부활체크
//	if (GetNetStatus() == NET_AIMASTER)
//	{
//		GetNetGameMode()->CheckNetAIPlayerResurrect(this);
//	}
//
//	if (!IsAIControlled() && GetNetStatus() == NET_SLAVE && !HasRootMotion())
//		AdjustLocationFromNet(DeltaSeconds);
//
//	ClientRootMotionParams.Clear();
}

void ABladeIINetPlayer::CheckSendMove()
{
	/*if (!bRootMotionBegun && ClientRootMotionParams.bHasRootMotion)
	{
		bRootMotionBegun = true;

		if (GetNetStatus() != NET_SLAVE && !bTemporarilyNetOff)
		{
			FString encoded_string = ToMessageHelper::MoveMessage(this, GetNetId(), 0, packet::ALLBUTME);
			SendMessage(encoded_string);
		}

		NetTimer = 0;

		return;
	}

	if (bRootMotionBegun && !ClientRootMotionParams.bHasRootMotion)
	{
		bRootMotionBegun = false;

		if(GetNetStatus() != NET_SLAVE && !bTemporarilyNetOff)
		{
			FString encoded_string = ToMessageHelper::MoveMessage(this, GetNetId(), 0, packet::ALLBUTME);
			SendMessage(encoded_string);
		}

		NetTimer = 0;

		return;
	}

	CheckOldVelocity();	*/
}

void ABladeIINetPlayer::CheckOldVelocity()
{
//	if (GetNetStatus() == NET_SLAVE)
//		return;
//
//	FVector fVelocity = GetVelocity();
//
//	if (!HasRootMotion())
//	{
//		if ((fVelocity.Size() == 0 && OldVelocity.Size() != 0)
//			|| (fVelocity.Size() != 0 && OldVelocity.Size() == 0))
//		{
//			FString encoded_string = ToMessageHelper::MoveMessage(this, GetNetId(), 0, packet::ALLBUTME);
//			SendMessage(encoded_string);
//
//			NetTimer = 0;
//		}
//	}
//
//	OldVelocity = fVelocity;
}

bool ABladeIINetPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	bool bCanTakeDamage = Super::CanTakeDamage(DamageCauser, DamageEvent, DamageType);
	//if (Role == ROLE_SimulatedProxy)
	//	return bCanTakeDamage && GotRemoteTakeDamage && EnableTakeDamageForNet;

	return bCanTakeDamage;
}

float ABladeIINetPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	BII_CHECK(GetNetStatus() != NET_NONE);

	auto*	GameMode			= GetNetGameMode();

	const float	ActualDamage	= Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//const int32 FinalHealth		= GetHealth();

	//const FDamageInfo* DamageInfo	= static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
	//const uint32 DamageHash			= DamageInfo ? DamageInfo->GetDamageHash() : 0;

	//if (GetNetStatus() != NET_SLAVE)
	//{
	//	packet::ByteStream payload;

	//	payload << GetNetId();
	//	payload << GetNetIdFrom(GetAttacker(DamageCauser));
	//	payload << ActualDamage;
	//	payload << DamageHash;
	//	payload << FinalHealth;

	//	FString encoded_string = packet::FinalizePacket(packet::TAKEDAMAGE, GetNetId(), 0, packet::ALLBUTME, payload);
	//	SendMessage(encoded_string);
	//	UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [TakeDamage] DamageHash : %f"), GetNetId(), DamageHash);

	//	// Damage 0일때 확인용 Log - (점령전 무적)
	//	if (ActualDamage == 0.f)
	//	{
	//		const bool IsValidDamageType = DamageInfo != nullptr;

	//		// ActualDamage 를 0으로 고정시킬 수 있는 요소들

	//		const bool IsValidEnemy			= GetAttacker(DamageCauser) != nullptr;
	//		const bool HasSuperArmor		= IsBuffActive(EBuffType::Buff_Unbeatable);
	//		const bool InvinsibleVariable	= bInvincible; // 무적버프 + @ (특정 모드의 연출 상황)으로 무적상태의 총괄개념

	//		// 1. 공격자를 찾을 수 없을경우  
	//		// 2. 무적 버프가 없는데 Invinsible인 경우(의도한 상황일 수 있으나 확률이 낮으므로 일단 Log보냄)
	//		const bool IsUnexpectedZeroDamage = (IsValidEnemy == false) || (InvinsibleVariable && !HasSuperArmor);
	//		if (IsUnexpectedZeroDamage)
	//		{
	//			const FString AttrString = FString::Printf(TEXT("%d%d%d"),
	//				static_cast<int32>(IsValidEnemy),
	//				static_cast<int32>(InvinsibleVariable),
	//				static_cast<int32>(HasSuperArmor));

	//			B2ActionLogSender Sender;
	//			Sender.setCategory(B2ActionLogType<EB2GameMode>::GetName(GetB2GameModeType(this)))
	//				.setAction(TEXT("ZeroActualDamage"))
	//				.setLabel(IsValidDamageType ? TEXT("ValidDamage") : TEXT("InvalidDamage"))
	//				.setActionAttr1(AttrString)
	//				.LogSender(eLogSendToFlag::LOG_SENDTO_GAMESERVER);
	//		}
	//	}

	//}

	//ReadyToTakeDamage(false);

	return ActualDamage;
}

bool ABladeIINetPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	check(GetNetStatus() != NET_NONE);

	//if (Role == ROLE_Authority || GotRemoteDyingSignal)
	//{
	//	return Super::CanDie(KillingDamage, DamageEvent, Killer, DamageCauser);
	//}

	return false;
}

bool ABladeIINetPlayer::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
#if WITH_EDITOR // In the case of editor preview however..
	extern bool GbEditorPVPGamePreview;
	if (GIsEditor && GbEditorPVPGamePreview)
	{
		Health = 0.0f;
		AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetGameMode());
		if (PVPGM)
		{
			PVPGM->HandleEditorPVPPreviewRemoteDead();
		}
		return true;
	}

	extern bool g_IsEditorRaidGamePreview;
	if (GIsEditor && g_IsEditorRaidGamePreview)
	{
		Health = 0.0f;
		if (AB2RaidGameMode* RaidGM = Cast<AB2RaidGameMode>(GetGameMode()))
			RaidGM->SetupEditorRaidPreviewRemoteDead();

		return true;
	}
#endif

	const bool bDie = Super::Die(KillingDamage, DamageEvent, Killer, DamageCauser);
	/*if( bDie && Role == ROLE_Authority)
	{
		packet::ByteStream payload;

		const uint32 KillerNetId = GetNetIdFrom(GetAttacker(DamageCauser));
		const int32 KilledClass = PCClassToInt(GetCurrentPlayerClass());
		
		payload << GetNetId();
		payload << KillerNetId;
		payload << KilledClass;
		
		FString encoded_string = packet::FinalizePacket(packet::DIE, GetNetId(), 0, packet::ALLBUTME, payload);
		SendMessage(encoded_string);
		UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [Die] KillerNetId : %d KilledClass : %d"), GetNetId(), KillerNetId, KilledClass);
		return true;
	}*/

	return bDie;
}

void ABladeIINetPlayer::PossessedBy(AController* InController)
{
	BII_CHECK(InController);

	if (GetNetStatus() == NET_SLAVE)
	{
		Controller = InController;
		PossessedByAI = true;
		return;
	}

	Super::PossessedBy(InController);
}

void ABladeIINetPlayer::FellOutOfWorld(const class UDamageType& dmgType)
{
	//if (GetB2GameModeType(this) == EB2GameMode::Raid && Role == ROLE_Authority)
	//{
	//	const FVector CurrLocation = GetActorLocation();
	//	const float OffsetZ = LastResurrectLocation.Z + 50.f;

	//	FVector TeleportLoc(CurrLocation.X, CurrLocation.Y, OffsetZ);
	//	SetActorLocation(TeleportLoc);
	//}

	//else
	//{
	//	Super::FellOutOfWorld(dmgType);
	//}
}

void ABladeIINetPlayer::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	check(GetNetStatus() != NET_NONE);
	
	if (GetNetStatus() == NET_SLAVE)
		return;

	Super::ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
}

void ABladeIINetPlayer::SetClientRootMotionParams(const FRootMotionMovementParams& RMMP)
{
	ClientRootMotionParams = RMMP;
}

void ABladeIINetPlayer::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser)
{
	check(GetNetStatus() != NET_NONE);

	Super::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);
	
	//switch (GetB2GameModeType(GetWorld()))
	//{
	//case EB2GameMode::Raid:
	//{
	//	if (SpawnResurrectionZoneTimer.IsValid())
	//		GetWorldTimerManager().ClearTimer(SpawnResurrectionZoneTimer);

	//	if (ResurrectionCount < AB2RaidGameMode::MAX_RESURRECTION_COUNT)
	//		GetWorldTimerManager().SetTimer(SpawnResurrectionZoneTimer, this, &ABladeIINetPlayer::SpawnHelpResurrectionZone, 2.0f, false);
	//	
	//	break;
	//}
	//case EB2GameMode::Control: //컨트롤은 필요시 따로 구현
	//default: //[@AKI, 170629] Raid, Tag, Control 제외하고 현재 BladeIINetPlayer를 사용하는 경우가 없는 걸로 알고 있음
	//	break;
	//}
}

void ABladeIINetPlayer::HealingHPByAmount(float InHealAmount, bool bUseEffect, bool bUseTextEffect)
{
	Super::HealingHPByAmount(InHealAmount, bUseEffect, bUseTextEffect);

	//if (GetNetStatus() != NET_SLAVE)
	//{
	//	packet::ByteStream	payload;
	//	payload << GetNetId();
	//	payload << Health;
	//	payload << GetNetId();

	//	FString encoded_string = packet::FinalizePacket(packet::HEALTH, NetId, 0, packet::ALLBUTME, payload);
	//	SendMessage(encoded_string);
	//}
}

void ABladeIINetPlayer::SetHealth(float NewHealth, bool bReceivedFromHost /*= false*/)
{
	if (GetLocalRole() == ROLE_Authority || bReceivedFromHost)
	{
		Super::SetHealth(NewHealth, bReceivedFromHost);
	}
}

bool ABladeIINetPlayer::RequestDamage(const float Damage, const FDamageInfo* DamageInfo, ABladeIICharacter* DamageCauser, bool NetBroadcast)
{
	// !!!중요!!!!! this == Victim
	// 공격자도 피해자도 Authority일 경우는 굳이 broadcasting 할 필요 없음

	bool bCanHandleDamage = Super::RequestDamage(Damage, DamageInfo, DamageCauser);
	if(bCanHandleDamage == false)
	{
		const uint32 DamageHash = DamageInfo ? DamageInfo->GetDamageHash() : 0;
		packet::ByteStream payload;

		payload << GetNetId();
		payload << GetNetIdFrom(GetAttacker(DamageCauser));
		payload << Damage;
		payload << DamageHash;

		FString encoded_string = packet::FinalizePacket(packet::REQUESTDAMAGE, GetNetId(), 0, packet::ALLBUTME, payload);
		//SendMessage(encoded_string);
		UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [RequestDamage] DamageHash : %f"), GetNetId(), DamageHash);

		// 여기오는게 오히려 false 리턴임 ( 피격 주체가 Local이 아니라 Damage handling이 안된다는 뜻 )
		return false;
	}

	return true;
}

bool ABladeIINetPlayer::ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	//if (Super::ProcessEtherSetEffect(EtherSetType, EtherCauser, CauserDamageInfo, ActualDamage))
	//{
	//	const uint32 EtherCauserNetId = GetNetIdFrom(EtherCauser);
	//	if (UB2EtherSetEffect* EtherObject = GetEtherSetEffect(EtherSetType))
	//	{
	//		packet::ByteStream payload;
	//		payload << GetNetId();
	//		payload << EtherCauserNetId;
	//		payload << EtherObject->GetEtherSetID();

	//		FString encoded_string = packet::FinalizePacket(packet::ACTIVATE_ETHER, GetNetId(), 0, packet::ALLBUTME, payload);
	//		SendMessage(encoded_string);

	//		UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [ACTIVATE_ETHER] EtherSetID : %d"), GetNetId(), EtherObject->GetEtherSetID());
	//		return true;
	//	}
	//}

	return false;
}

void ABladeIINetPlayer::RequestEtherSetEffect(uint32 InEtherCauserNetId, int32 InEtherSetID)
{
	packet::ByteStream payload;
	payload << GetNetId();
	payload << InEtherCauserNetId;
	payload << InEtherSetID;

	FString encoded_string = packet::FinalizePacket(packet::ACTIVATE_ETHER, GetNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage(encoded_string);

	UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [ACTIVATE_ETHER] EtherSetID : %d"), GetNetId(), InEtherSetID);
}

//class UB2Buff_Base* ABladeIINetPlayer::AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* BuffCauser, bool bUseEffect/* = true*/, bool bUseTextEffect/* = true*/, TArray<float>* PtrAdditionalParams/* = nullptr*/)
//{
//	if (Role == ROLE_Authority)
//	{
//		const uint32 CauserNetId = GetNetIdFrom(BuffCauser);
//
//		packet::ByteStream payload;
//		payload << GetNetId();
//		payload << CauserNetId;
//		payload << uint8(BuffType);
//		payload << fDuration;
//		payload << Amount;
//		payload << bUseEffect;
//		payload << bUseTextEffect;
//
//		if(!PtrAdditionalParams)
//			payload << int32(0);
//		else
//		{
//			int32 nParamsCnt = (*PtrAdditionalParams).Num();
//			payload << nParamsCnt;
//
//			for (int32 nCnt = 0; nCnt < nParamsCnt; ++nCnt)
//			{
//				payload << (*PtrAdditionalParams)[nCnt];
//			}
//		}
//
//		FString encoded_string = packet::FinalizePacket(packet::ACTIVATE_BUFF, GetNetId(), 0, packet::ALLBUTME, payload);
//		SendMessage(encoded_string);
//
//		return Super::AddBuff(BuffType, fDuration, Amount, BuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);
//	}
//
//	return nullptr;
//}

void ABladeIINetPlayer::SubscribeEvents()
{
	check(GetNetStatus() != NET_NONE);

	if (bEventsSubscribed)
		return;

	CAPTURE_UOBJECT(ABladeIINetPlayer);

	if (GetNetStatus() == NET_MASTER)
	{
		Super::SubscribeEvents();
	}
	else if (GetNetStatus() == NET_SLAVE)
	{
		ReceivedMovementTicket = ReceivedMovementClass<uint32, FVector, FVector, FRotator, float>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedMovement, uint32 netid, FVector pos, FVector vel, FRotator rot, float fCurrentSpeed)
				if (!Capture->CanHandlePacket(netid))
					return;
				if (Capture->IsInQTEState())
					return;
// 				if (Capture->HasRootMotion())
// 					Capture->UpdateRotation(rot);
// 				else
					Capture->UpdateLocation(pos, vel, rot, fCurrentSpeed);
			END_CAPTURE_OBJECT()
		);
		ReceivedRotationTicket = ReceivedRotationClass<uint32, FRotator>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedRotation, uint32 netid, FRotator rot)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->UpdateRotation(rot);
			END_CAPTURE_OBJECT()
		);
		ReceivedAttackTicket = ReceivedAttackClass<uint32, FVector, FRotator>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedAttack, uint32 netid, FVector pos, FRotator rot)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteStartAttack(pos, rot);
			END_CAPTURE_OBJECT()
		);
		ReceivedStopAttackTicket = ReceivedStopAttackClass<uint32, FVector, FRotator>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedStopAttack, uint32 netid, FVector pos, FRotator rot)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteStopAttack(pos, rot);
			END_CAPTURE_OBJECT()
		);
// 		ReceivedGuardTicket = ReceivedGuardClass<uint32, FVector, FRotator, uint32, uint32, uint8, uint8>::GetInstance().Subscribe(
// 			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedGuard, uint32 netid, FVector pos, FRotator rot, uint32 SkillCancel, uint32 NeedUpdateTarget, uint8 GuardBreakType, uint8 attackState)
// 				if (!Capture->CanHandlePacket(netid))
// 					return;
// 				Capture->RemoteStartGuard(pos, rot, SkillCancel, NeedUpdateTarget, GuardBreakType, attackState);
// 			END_CAPTURE_OBJECT()
// 		);
// 		ReceivedStopGuardTicket = ReceivedStopGuardClass<uint32, FVector, FRotator>::GetInstance().Subscribe(
// 			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedStopGuard, uint32 netid, FVector pos, FRotator rot)
// 				if (!Capture->CanHandlePacket(netid))
// 					return;
// 				Capture->RemoteStopGuard(pos, rot);
// 			END_CAPTURE_OBJECT()
// 		);
		ReceivedSkillAttackTicket = ReceivedSkillAttackClass<uint32, uint8, uint8, FVector, FRotator>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedSkillAttack, uint32 netid, uint8 index, uint8 anim_index, FVector pos, FRotator rot)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteSkillAttack(index, anim_index, pos, rot);
			END_CAPTURE_OBJECT()
		);
		ReceivedTagPlayerTicket = ReceivedTagPlayerClass<uint32, uint8, uint8, FVector, FRotator>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedTagPlayer, uint32 netid, uint8 state, uint8 Class, FVector pos, FRotator rot)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteStartTag(static_cast<ETagAttackMode>(state), IntToPCClass(Class), pos, rot);
			END_CAPTURE_OBJECT()
		);
		ReceivedComboEndTicket = ReceivedComboEndClass<uint32, uint8, bool>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedComboEnd, uint32 netid, uint8 InLastAttackState, bool IsAttackCanceled)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteComboEnd(InLastAttackState, IsAttackCanceled);
			END_CAPTURE_OBJECT()
		);
		ReceivedTakeDamageTicket = ReceivedTakeDamageClass<uint32, uint32, float, uint32, int32, int32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedTakeDamage, uint32 Victim_netid, uint32 Damagecauser_netid, float DamageAmount, uint32 Hash_Damageinfo, int32 Victim_Health, int32 ReceivedDamageNum)
				if (!Capture->CanHandlePacket(Victim_netid))
					return;
				Capture->RemoteTakeDamage(DamageAmount, Hash_Damageinfo, Damagecauser_netid, Victim_Health, ReceivedDamageNum);
			END_CAPTURE_OBJECT()
			);
		ReceivedRequestDamageTicket = ReceivedRequestDamageClass<uint32, uint32, float, uint32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedRequestDamage, uint32 Victim_netid, uint32 Damagecauser_netid, float DamageAmount, uint32 Hash_Damageinfo)
				if (!Capture->CanHandlePacket(Victim_netid))
					return;
					Capture->RemoteRequestDamage(DamageAmount, Hash_Damageinfo, Damagecauser_netid);
			END_CAPTURE_OBJECT()
			);
		ReceivedHealthTicket = ReceivedHealthClass<uint32, float, uint32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedHealth, uint32 netid, float health, uint32 DamageCauserNetId)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteAdjustHealth(health, DamageCauserNetId);
			END_CAPTURE_OBJECT()
		);
		ReceivedUpdateTargetTicket = ReceivedUpdateTargetClass<uint32, uint32, float>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedUpdateTarget, uint32 netid, uint32 TargetId, float Dist)
				if (!Capture->CanHandlePacket(netid))
					return;
				Capture->RemoteUpdateTarget(TargetId, Dist);
			END_CAPTURE_OBJECT()
		);
// 		ReceivedCounterAttackTicket = ReceivedCounterAttackClass<uint32, FVector, FRotator>::GetInstance().Subscribe(
// 			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedCounterAttack, uint32 netid, FVector pos, FRotator rot)
// 				if (!Capture->CanHandlePacket(netid))
// 					return;
// 				Capture->RemoteCounterAttack(pos, rot);
// 			END_CAPTURE_OBJECT()
// 		);
		ReceivedEtherActivatedTicket = ReceivedEtherActivatedClass<uint32, uint32, uint32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedEtherActivated, uint32 OwnerNetId, uint32 CauserNetId, uint32 EtherId)
				if (!Capture->CanHandlePacket(OwnerNetId))
					return;
				Capture->RemoteEtherActivated(OwnerNetId, CauserNetId, EtherId);
			END_CAPTURE_OBJECT()
		);
		ReceivedDieTicket = ReceivedDieClass<uint32, uint32, int32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedDie, uint32 OwnerNetId, uint32 KillerNetId, int32 KilledClass)
				if (!Capture->CanHandlePacket(OwnerNetId))
					return;
				Capture->RemoteDie(OwnerNetId, KillerNetId, IntToPCClass(KilledClass));
			END_CAPTURE_OBJECT()
		);
		ReceivedBuffActivatedTicket = ReceivedBuffActivatedClass<uint32, uint32, uint8, float, float, bool, bool, TArray<float>*>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedBuffActivated, uint32 BuffOwnerId, uint32 BuffCauserId, uint8 BuffType, float fDuration, float fAmount, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams)
				if (!Capture->CanHandlePacket(BuffOwnerId))
					return;
				Capture->RemoteAddBuff(BuffOwnerId, BuffCauserId, EBuffType(BuffType), fDuration, fAmount, bUseEffect, bUseTextEffect, PtrAdditionalParams);
			END_CAPTURE_OBJECT()
		);

		ReceivedSetAnimStateTicket = ReceivedSetAnimStateClass<uint32, const FString&>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedSetAnimState, uint32 netId, const FString& ReceivedAnimState)
				if (!Capture->CanHandlePacket(netId))
					return;
				Capture->RemoteSetAnimState(netId, ReceivedAnimState);
			END_CAPTURE_OBJECT()
		);
		ReceivedChangeDamageNumTicket = ReceivedChangeDamageNumClass<uint32, int32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedChangeDamageNum, uint32 netId, int32 NewDamageNum)
			if (!Capture->CanHandlePacket(netId))
				return;
			Capture->RemoteSetDamageNum(netId, NewDamageNum);
			END_CAPTURE_OBJECT()
		);
		ReceivedSyncAnimStateIndexTicket = ReceivedSyncAnimStateIndexClass<uint32, FNetSyncAnimInfo&>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedSyncAnimStateIndex, uint32 netId, FNetSyncAnimInfo& NetSyncAnimInfo)
			if (!Capture->CanHandlePacket(netId))
				return;
		Capture->ReceiveAnimStateIndex(NetSyncAnimInfo);
		END_CAPTURE_OBJECT()
			);
		ReceivedQteNextTicket = ReceivedQteNextClass<uint32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedQteNext, uint32 netId)
			if (!Capture->CanHandlePacket(netId))
				return;
		Capture->ReceiveRemoteQteNext();
		END_CAPTURE_OBJECT()
			);
		ReceivedSpawnEndTicket = ReceivedSpawnEndClass<uint32>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(ReceivedSpawnEnd, uint32 netId)
			if (!Capture->CanHandlePacket(netId))
				return;
		Capture->RemoteOnSpawnEnd();
		END_CAPTURE_OBJECT()
			);

		
		
		
	}

	bEventsSubscribed = true;
}

void ABladeIINetPlayer::UnsubscribeEvents()
{
	if (!bEventsSubscribed)
		return;

	if (GetNetStatus() == NET_SLAVE)
	{
		ReceivedMovementClass<uint32, FVector, FVector, FRotator, float>::GetInstance().Unsubscribe(ReceivedMovementTicket);
		ReceivedRotationClass<uint32, FRotator>::GetInstance().Unsubscribe(ReceivedRotationTicket);
		ReceivedAttackClass<uint32, FVector, FRotator>::GetInstance().Unsubscribe(ReceivedAttackTicket);
		ReceivedStopAttackClass<uint32, FVector, FRotator>::GetInstance().Unsubscribe(ReceivedStopAttackTicket);
// 		ReceivedGuardClass<uint32, FVector, FRotator, uint32, uint32, uint8, uint8>::GetInstance().Unsubscribe(ReceivedGuardTicket);
// 		ReceivedStopGuardClass<uint32, FVector, FRotator>::GetInstance().Unsubscribe(ReceivedStopGuardTicket);
		ReceivedSkillAttackClass<uint32, uint8, uint8, FVector, FRotator>::GetInstance().Unsubscribe(ReceivedSkillAttackTicket);
		ReceivedTagPlayerClass<uint32, uint8, uint8, FVector, FRotator>::GetInstance().Unsubscribe(ReceivedTagPlayerTicket);
		ReceivedComboEndClass<uint32, uint8, bool>::GetInstance().Unsubscribe(ReceivedComboEndTicket);
		ReceivedTakeDamageClass<uint32, uint32, float, uint32, int32, int32>::GetInstance().Unsubscribe(ReceivedTakeDamageTicket);
		ReceivedRequestDamageClass<uint32, uint32, float, uint32>::GetInstance().Unsubscribe(ReceivedRequestDamageTicket);
		ReceivedHealthClass<uint32, float, uint32>::GetInstance().Unsubscribe(ReceivedHealthTicket);
		ReceivedUpdateTargetClass<uint32, uint32, float>::GetInstance().Unsubscribe(ReceivedUpdateTargetTicket);
//		ReceivedCounterAttackClass<uint32, FVector, FRotator>::GetInstance().Unsubscribe(ReceivedCounterAttackTicket);
		ReceivedEtherActivatedClass<uint32, uint32, uint32>::GetInstance().Unsubscribe(ReceivedEtherActivatedTicket);
		ReceivedDieClass<uint32, uint32, int32>::GetInstance().Unsubscribe(ReceivedDieTicket);
		ReceivedBuffActivatedClass<uint32, uint32, uint8, float, float, bool, bool, TArray<float>*>::GetInstance().Unsubscribe(ReceivedBuffActivatedTicket);
		ReceivedSetAnimStateClass<uint32, const FString&>::GetInstance().Unsubscribe(ReceivedSetAnimStateTicket);
		ReceivedChangeDamageNumClass<uint32, int32>::GetInstance().Unsubscribe(ReceivedChangeDamageNumTicket);
		ReceivedSyncAnimStateIndexClass<uint32, FNetSyncAnimInfo&>::GetInstance().Unsubscribe(ReceivedSyncAnimStateIndexTicket);
		ReceivedQteNextClass<uint32>::GetInstance().Unsubscribe(ReceivedQteNextTicket);
		ReceivedSpawnEndClass<uint32>::GetInstance().Unsubscribe(ReceivedSpawnEndTicket);
	}
	else if (GetNetStatus() == NET_MASTER)
	{
		Super::UnsubscribeEvents();
	}

	bEventsSubscribed = false;
}

bool ABladeIINetPlayer::CanPlayerHaveMoveMotion() const
{
	if (GetNetStatus() == NET_MASTER)
		return Super::CanPlayerHaveMoveMotion();

	return GetDamagedNum() == 0 && GetAttackState() == EAttackState::ECS_None; 
}

bool ABladeIINetPlayer::IsFineToStartAnyNewInputHandling()
{
	check(GetNetStatus() != NET_NONE);

	if (GetNetStatus() == NET_MASTER)
		return Super::IsFineToStartAnyNewInputHandling();

	return IsAlive();
}

bool ABladeIINetPlayer::IsFineToStartAnyNewMoveInputHandling()
{
	check(GetNetStatus() != NET_NONE);

	if (GetNetStatus() == NET_MASTER)
		return Super::IsFineToStartAnyNewMoveInputHandling();

	if (!GetAbleToMove())
		return false;

	return IsAlive();
}

void ABladeIINetPlayer::MoveForward(float Value)
{
	IsInvalidateInput = false;

	Super::MoveForward(Value);

	IsInvalidateInput = GetLastMovementInputVector() != GetPendingMovementInputVector();
}

void ABladeIINetPlayer::MoveRight(float Value)
{
	IsInvalidateInput = false;

	Super::MoveRight(Value);

	IsInvalidateInput = GetLastMovementInputVector() != GetPendingMovementInputVector();
}

void ABladeIINetPlayer::StartAttack()
{
	//check(GetNetStatus() != NET_NONE);

	//if (GetNetStatus() == NET_SLAVE)
	//{
	//	if (!IsAlive())
	//		return;

	//	Super::StartAttack();

	//	return;
	//}

	//FString encoded_string = ToMessageHelper::AttackMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//SendMessage(encoded_string);
	////UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [ATTACK]"), GetNetId());

	//Super::StartAttack();
}

void ABladeIINetPlayer::StopAttack()
{
	if (GetNetStatus() == NET_SLAVE)
	{
		Super::StopAttack();
		return;
	}

	//FString encoded_string = ToMessageHelper::StopAttackMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//SendMessage(encoded_string);
	////UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [STOPATTACK]"), GetNetId());
	//
	//Super::StopAttack();
}

EAttackState ABladeIINetPlayer::StartSkillInner(int32 index, bool IsPlayerInput)
{
	BII_CHECK(CachedSkillRTManager);

	EAttackState ResultAttackState = Super::StartSkillInner(index, IsPlayerInput);

	//if (ResultAttackState != EAttackState::ECS_None && GetLocalRole() == ROLE_Authority)
	//{
	//	FString encoded_string = ToMessageHelper::StartSkillMessage(this, GetNetId(), 0, packet::ALLBUTME, ResultAttackState, CurrentSkillAnimIdx);
	//	SendMessage(encoded_string);
	//	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [STARTSKILL]"), GetNetId());
	//}

	return ResultAttackState;
}

void ABladeIINetPlayer::StartTagCommon()
{
	if (CanTag() == false)
		return;

	Super::StartTagCommon();

	bTemporarilyNetOff = true;
}

void ABladeIINetPlayer::StartDash(float InDashDuration, float InMaxDistance)
{
	check(GetNetStatus() != NET_NONE);

	if (GetNetStatus() == NET_SLAVE)
		// Assumed velocity sync will be just by network packets
		return;

	Super::StartDash(InDashDuration, InMaxDistance);
}

void ABladeIINetPlayer::StopDash()
{
	check(GetNetStatus() != NET_NONE);
	if (GetNetStatus() == NET_SLAVE)
		// Assumed velocity sync will be just by network packets
		return;

	Super::StopDash();
}

// void ABladeIINetPlayer::OnCounterStart()
// {
// 	bForceCounterAttack = false;
// 
// 	if(GetNetStatus() == NET_SLAVE)
// 		return;
// 
// // 	FString encoded_string = ToMessageHelper::CounterAttackMessage(this, GetNetId(), 0, packet::ALLBUTME);
// // 	SendMessage(encoded_string);
// 
// 	Super::OnCounterStart();
// }

EPCSpawnMotionState ABladeIINetPlayer::GetSpawnMotionState()
{
	if (GetNetStatus() == NET_SLAVE)
		return RemoteSpawnMotionState;

	return Super::GetSpawnMotionState();
}

void ABladeIINetPlayer::EndSpawnMotionState()
{
	check(GetNetStatus() != NET_NONE);
	if (GetNetStatus() == NET_SLAVE)
	{
		RemoteSpawnMotionState = EPCSpawnMotionState::EPST_Normal;
		return;
	}

	Super::EndSpawnMotionState();
}

float ABladeIINetPlayer::UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled)
{
	float TargetDist = Super::UpdateTarget(bInputGuided, bOnlyQTEEnabled);

	if (GetNetStatus() == NET_SLAVE)
		return 0.0f;

	//if (TargetActor != OldTargetActor)
	//{
	//	auto Id = GetNetIdFrom(TargetActor);

	//	FString encoded_string = ToMessageHelper::UpdateTargetMessage(this, GetNetId(), 0, packet::ALLBUTME, Id, TargetDist);
	//	SendMessage(encoded_string);

	//	OldTargetActor = TargetActor;
	//}

	return TargetDist;
}

void ABladeIINetPlayer::ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled)
{
	check(GetNetStatus() != NET_NONE);

	//if (GetNetStatus() == NET_MASTER || GetNetStatus() == NET_AIMASTER)
	//{
	//	FString encoded_string = ToMessageHelper::ComboEndMessage(this, GetNetId(), 0, packet::ALLBUTME, static_cast<uint8>(InLastAttackState), IsAttackCanceled);
	//	SendMessage(encoded_string);
	//	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [COMBOEND]"), GetNetId());

	//	Super::ReceiveComboEnd(InLastAttackState);
	//}
}

void ABladeIINetPlayer::OnQTENext()
{
	//if (GetNetStatus() == NET_MASTER || GetNetStatus() == NET_AIMASTER)
	//{
	//	FString encoded_string = ToMessageHelper::QteNextMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//	SendMessage(encoded_string);
	//	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [COMBOEND]"), GetNetId());

	//	Super::OnQTENext();
	//}
}

//UB2SkillRTManager* ABladeIINetPlayer::GetSkillRTManager()
//{
//	check(GetNetStatus() != NET_NONE);
//
//	if (GetNetStatus() == NET_MASTER)
//		return Super::GetSkillRTManager();
//
//	if (CachedSkillRTManager != nullptr)
//		return CachedSkillRTManager;
//
//	CachedSkillRTManager = GetNetGameMode()->GetRemoteSkillRTManager(GetNetId());
//	CachedSkillRTManager->InitSkillRTManager(this);
//
//	return CachedSkillRTManager;
//}

ICharacterDataStore* ABladeIINetPlayer::GetCharacterDataStore()
{
	check(GetNetStatus() != NET_NONE);

	if (GetNetStatus() == NET_MASTER)
		return Super::GetCharacterDataStore();

	if (CharacterDataStore != nullptr)
		return CharacterDataStore;

	CharacterDataStore = GetNetGameMode()->GetRemotePlayerDataStore(GetNetId());

	return CharacterDataStore;
}

void ABladeIINetPlayer::NotifyPlayerDeadEnd()
{
	check(GetNetStatus() != NET_NONE);

	//Super::NotifyPlayerDeadEnd();

	//if (GetNetStatus() != NET_AIMASTER)
	//	return;

	//auto* MyController = Cast<ABladeIIPlayerAIController>(GetController());

	//if (MyController)
	//	MyController->StopAutoPlay();
}

void ABladeIINetPlayer::SetAttackState(EAttackState InAttackState)
{
	check(GetNetStatus() != NET_NONE);

	// ComboEnd에 의해서 정상적으로 상태가 종료되는 케이스에서는 ECS_None 상태로 바꿔 줘야한다
	if (InAttackState != EAttackState::ECS_None)
	{
		if (GetNetStatus() == NET_SLAVE)
		{
			if (GetAttackState() == EAttackState::ECS_TagOtherChar)
			{
				return;
			}
		}
	}

	Super::SetAttackState(InAttackState);
}

FVector ABladeIINetPlayer::GetVelocity() const
{
	return GetCharacterMovement()->Velocity;

	//return GetRootComponent()->ComponentVelocity;
}

FVector ABladeIINetPlayer::GetLocation() const
{
	return GetCharacterMovement()->GetActorLocation();
}

void ABladeIINetPlayer::OnEnterAnimState(const FName& NewState)
{
	Super::OnEnterAnimState(NewState);

	//if (Role == ROLE_Authority)
	//{
	//	/*
	//	// B2BUG-1616 - Packet 최적화.
	//	// Animation Simulate 동기화 때문에 넣은 기능이나 쓰지 않으므로 패킷 송신 X

	//	FString CurrAnimState = NewState.ToString();
	//	
	//	packet::ByteStream payload;
	//	payload << CurrAnimState;

	//	FString encoded_string = packet::FinalizePacket(packet::SET_ANIMSTATE, GetNetId(), 0, packet::ALLBUTME, payload);
	//	SendMessage(encoded_string);
	//	*/

	//	FString CurrAnimState = NewState.ToString();
	//	UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [OnChangedAnimState]: %s"), GetNetId(), *CurrAnimState);
	//}
}

void ABladeIINetPlayer::OnEnterSkillState_Tag(EPCClass TagClass)
{
	//Super::OnEnterSkillState_Tag(TagClass);

	//if (Role == ROLE_Authority)
	//{
	//	FString encoded_string = ToMessageHelper::TagPlayerMessage(this, GetNetId(), 0, packet::ALLBUTME, TagAttackMode, TagClass);
	//	SendMessage(encoded_string);
	//}
}

void ABladeIINetPlayer::OnExitAnimState(const FName& PrevState)
{

}

bool ABladeIINetPlayer::IsRemoteCharacter()
{
	return !IsAIControlled() && GetNetStatus() == NET_SLAVE;
}

void ABladeIINetPlayer::SendAnimStateIndex(int32 NewStateIndex)
{
	if (NetId == 0)
		return;

	//if (GetNetStatus() == NET_MASTER || GetNetStatus() == NET_AIMASTER)
	//{		
	//	m_nLastSendAnimStateIndex = NewStateIndex;

	//	auto location = GetLocation();
	//	auto velocity = GetVelocity();
	//	auto rotation = GetActorRotation();

	//	packet::ByteStream payload;

	//	// 애님싱크정보 struct FNetSyncAnimInfo 모양
	//	
	//	uint8 eTargetPCClass = static_cast<uint8>(GetCurrentPlayerClass());
	//	
	//	bool bSmash = GetAnimInstance()->IsSmash();
	//	bool bDamaged = GetAnimInstance()->IsDamaged();
	//	bool bIsInQTEState = IsInQTEState();
	//	uint8 ePCSpawnMotionState = static_cast<uint8>(GetAnimInstance()->GetSpawnMotionState());
	//	uint8 eDamageAnimIndex = static_cast<uint8>(GetAnimInstance()->GetDamageAnimIndex());
	//	uint8 eGuardBreakLogic = static_cast<uint8>(LastGuardBreakType);//static_cast<uint8>(GetAnimInstance()->GetGuardBreakLogic());
	//	uint8 eQTEType = static_cast<uint8>(GetAnimInstance()->GetQTEType());
	//	uint8 eQTENPCClass = static_cast<uint8>(GetAnimInstance()->GetQTENPCClass());
	//	uint8 eAttackState = static_cast<uint8>(GetAnimInstance()->GetAnimAttackState());

	//	uint8 eSkillAnimType = static_cast<uint8>(Super::GetCurrentSKillAnimIdx_Native());
	//	uint8 nQTEStateNum = static_cast<uint8>(GetAnimInstance()->GetQTEStateNum());

	//	payload << eTargetPCClass;
	//	payload << NewStateIndex;

	//	payload << bSmash;
	//	payload << bDamaged;
	//	payload << bIsInQTEState;
	//	payload << ePCSpawnMotionState;
	//	payload << eDamageAnimIndex;
	//	payload << eGuardBreakLogic;
	//	payload << eQTEType;
	//	payload << eQTENPCClass;
	//	payload << eAttackState;
	//	payload << eSkillAnimType;
	//	payload << nQTEStateNum;

	//	// 위치정보
	//	packet::PackingHelper(payload, location);
	//	packet::PackingHelper(payload, velocity);
	//	packet::PackingHelper(payload, rotation);

	//	float fCurrentSpeed = GetAnimInstance()->GetCurrentSpeed();
	//	payload << fCurrentSpeed;
	//	UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::SendAnimStateIndex(), Name = %s, NetID = %d, Class = %d, AttackState = %d, PCSpawnMotionState = %d"), 
	//		*GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), eAttackState, ePCSpawnMotionState);
	//	FString encoded_string = packet::FinalizePacket(packet::SYNC_ANIMSTATEINDEX, GetNetId(), 0, packet::ALLBUTME, payload);
	//	SendMessage(encoded_string);
	//}
}

void ABladeIINetPlayer::SendAnimStateIndex()
{
	if (m_nLastSendAnimStateIndex != -1)
	{
		//UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::SendAnimStateIndex, Name = %s, NetID = %d, Class = %d, m_nLastSendAnimStateIndex = %d"),
		//	*GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), m_nLastSendAnimStateIndex);
		SendAnimStateIndex(m_nLastSendAnimStateIndex);
	}
}

void ABladeIINetPlayer::ForceSendAnimStateIndex()
{
	SendAnimStateIndex(1);
}

void ABladeIINetPlayer::ReceiveAnimStateIndex(FNetSyncAnimInfo& NetSyncAnimInfo)
{
	UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::ReceiveAnimStateIndex, Name = %s Netid = %d, Class = %d, AttackState = %d, PCSpawnMotionState = %d"), 
		*GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), NetSyncAnimInfo.AttackState, NetSyncAnimInfo.PCSpawnMotionState);
	SetSyncedAnimInfo(NetSyncAnimInfo);
}

int32 ABladeIINetPlayer::GetSyncedAnimStateIndex()
{
	int32 nResult = -1;
	//FNetSyncAnimInfo AnimSyncInfo;
	//if (GetSyncedAnimInfo(AnimSyncInfo))
	//{
	//	nResult = AnimSyncInfo.nSyncedAnimStateIndex;
	//	GetAnimInstance()->SetSmash(AnimSyncInfo.bSmash);
	//	GetAnimInstance()->SetDamaged(AnimSyncInfo.bDamaged);
	//	GetAnimInstance()->SetSpawnMotionState(static_cast<EPCSpawnMotionState>(AnimSyncInfo.PCSpawnMotionState));
	//	GetAnimInstance()->SetDamageAnimIndex(static_cast<EAttackType>(AnimSyncInfo.DamageAnimIndex));
	//	GetAnimInstance()->SetGuardBreakLogic(static_cast<EGuardBreakLogicType>(AnimSyncInfo.GuardBreakLogic));
	//	GetAnimInstance()->SetQTEType(static_cast<EQTEType>(AnimSyncInfo.QTEType));
	//	GetAnimInstance()->SetQTENPCClass(static_cast<ENPCClass>(AnimSyncInfo.QTENPCClass));
	//	GetAnimInstance()->SetAnimAttackState(static_cast<EAttackState>(AnimSyncInfo.AttackState));
	//	m_SkillAnimTypeForRemotePlayer = static_cast<ESkillAnimType>(AnimSyncInfo.SkillAnimType);
	//	SetQTEStateFlag(AnimSyncInfo.bInQTEState);
	//	SetQTEStateNum(static_cast<int32>(AnimSyncInfo.QTEStateNum));

	//	//UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::GetSyncedAnimStateIndex, Netid = %d, Class = %d, AttackState = %d, PCSpawnMotionState = %d"), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), AnimSyncInfo.AttackState, AnimSyncInfo.PCSpawnMotionState);
	//}

	return nResult;
}

void ABladeIINetPlayer::ReceiveRemoteQteNext()
{
	Super::OnQTENext();
}

void ABladeIINetPlayer::UpdateLocation(FVector pos, FVector vel, FRotator rot, float fCurrentSpeed)
{
	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [MOVE]"), GetNetId());
//
//	// 점령전에서 QTE허가는 호스트가하고, QTE주체도 호스트 허가를 받고 QTE진입한다.
//	// 이 둘 이외의 플레이어는 QTE플레이어가 QTE진입후에 이동패킷을 받을수도 있음.
//	// ex) 플레이어1 기기 QTE요청 - 호스트 허가 - 나머지 모든 기기 플레이어1 QTE실행
//	// - 플레이어1 기기 네트워크 지연으로 QTE실행안하구 이동패킷 전송후 QTE실행 
//	// - 나머지플레이어 기기 플레이어1 QTE실행후 이동및 로테이션 적용
//	if (IsInQTEState())
//		return;
//
//	check(GetNetStatus() == NET_SLAVE);
//	SetActorRotation(rot);
//
//#if !UE_BUILD_SHIPPING
//	UpdateLocationRadius = NetFakeConfigure::GetInstance().GetUpdateLocationRadius();
//#endif
//
//	if (!FVector::PointsAreNear(pos, GetActorLocation(), UpdateLocationRadius) || vel.Size() == 0)
//		SetActorLocation(pos);
//
//	if (!HasRootMotion())
//		UpdateVelocity(vel);
//
//	NetAdjustGoalLocation = pos;
//	NetAdjustGoalLocationTime = GetWorld()->GetTimeSeconds();
//
//	GetAnimInstance()->SetCurrentSpeed(fCurrentSpeed);
//
//	// 리모트는 전방 블로킹 보정용 변수도 채널통해 받은걸로 셋팅
//	m_vCachedLocationForRootMotionBlock = pos;
}

void ABladeIINetPlayer::AdjustLocationFromNet(float DeltaSeconds)
{
	float fMoveAmount = 100.0f * DeltaSeconds;
	FVector vDirection = GetNetAdjustGoalLocation() - GetActorLocation();
	float fDiff = vDirection.Size();

	if (fMoveAmount >= fDiff)
		SetActorLocation(GetNetAdjustGoalLocation());
	else
	{
		vDirection.Normalize();
		SetActorLocation(GetActorLocation() + (vDirection * fMoveAmount));
	}
}

void ABladeIINetPlayer::UpdateRotation(FRotator rot)
{
	BII_CHECK(NetStatus == NET_SLAVE);

	SetActorRotation(rot);
}

void ABladeIINetPlayer::RemoteStartAttack(FVector pos, FRotator rot)
{
	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [ATTACK]"), GetNetId());
	check(GetNetStatus() == NET_SLAVE);
	SyncLocationAndRotation(pos, rot);
	StartAttackFromInput();
}

void ABladeIINetPlayer::RemoteStopAttack(FVector pos, FRotator rot)
{
	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [STOPATTACK]"), GetNetId());
	check(GetNetStatus() == NET_SLAVE);
	//SyncLocationAndRotation(pos, rot);
	StopAttackFromInput();
}

// void ABladeIINetPlayer::RemoteStartGuard(FVector pos, FRotator rot, uint32 SkillCancel, uint32 NeedUpdateTarget, uint8 GuardBreak, uint8 attackState)
// {
// 	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [GUARD]"), GetNetId());
// 	check(GetNetStatus() == NET_SLAVE);
// 	//SyncLocationAndRotation(pos, rot);
// 
// 	// ABladeIIPlayer::StartGuard() 이용하지 않고 값만 설정해준다.
// 	bSkillCancel		= SkillCancel;
// 	bNeedUpdateTarget	= NeedUpdateTarget;
// 	LastGuardBreakType	= static_cast<EGuardBreakLogicType>(GuardBreak);
// 	SetAttackState(static_cast<EAttackState>(attackState));
// 
// 	// 가드 스타트면 머터리얼 변조
// // 	if (static_cast<EAttackState>(attackState) == EAttackState::ECS_GuardStart)
// // 	{
// // 		UB2BuffModeEffectInfo* BuffModeEffectInfo = GetBuffModeEffectInfo();
// // 		if (BuffModeEffectInfo)
// // 		{
// // 			BuffModeEffectInfo->GuardEffect(this);
// // 		}
// // 	}
// 
// 	Super::StartGuardFromInput();
// }

// void ABladeIINetPlayer::RemoteStopGuard(FVector pos, FRotator rot)
// {
// 	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [STOPGUARD]"), GetNetId());
// 	check(GetNetStatus() == NET_SLAVE);
// 	//SyncLocationAndRotation(pos, rot);
// 
// 	Super::StopGuardFromInput();
// }

void ABladeIINetPlayer::RemoteSkillAttack(uint8 state, uint8 anim_index, FVector pos, FRotator rot)
{
	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [STARTSKILL]"), GetNetId());
	check(GetNetStatus() == NET_SLAVE);
	SyncLocationAndRotation(pos, rot);

	//
	const EAttackState RemoteAttackState = static_cast<EAttackState>(state);
	//SetAttackState(RemoteAttackState); // Does not actually start by StartSkillByInputIndex call, must set the return value here.
	SetReservedSkill(RemoteAttackState);

	CurrentSkillAnimIdx = static_cast<ESkillAnimType>(anim_index);
	bLastInvokedAnySkillAttack = true;

	OnStartSkill(RemoteAttackState);

	// 스킬->스킬 스테이트전환없이 연결되는 케이스 생기면 요거 주석풀기
	//GetAnimInstance()->SetAnimAttackState(RemoteAttackState);
}

void ABladeIINetPlayer::RemoteStartTag(ETagAttackMode state, EPCClass CharClass, FVector pos, FRotator rot)
{
	if (CanTag() == false)
		return;

	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [TAGPLAYER]"), GetNetId());
	check(GetNetStatus() == NET_SLAVE);
	SyncLocationAndRotation(pos, rot);

	if (GetTagPlayerClass() != CharClass)
	{
		//어느 순간 태그캐릭터가 꼬인 상태...
		//이 명령을 무시한다.

		check(this->CurrentPlayingClass == CharClass);
		return;
	}

	TagAttackMode = state;
	StartTagCommon();

	// 애님스테이트 셋팅. 스킬스테이트에서 스테이트 변경없이 플래그만 변경되는 케이스가 있다.
	// 상대쪽 로컬플레이어에서 SendAnimStateIndex를 못쏴주니 플래그라도 셋팅해줌.
	// 정상적으로 스테이트변경으로인해 태그(스킬 스테이트) AnimStateIndex를 받으면 아래정보는 덮어쓸거임
	//GetAnimInstance()->SetAnimAttackState(EAttackState::ECS_TagOtherChar);
}

void ABladeIINetPlayer::RemoteComboEnd(uint8 InLastAttackState, bool IsAttackCanceled)
{
	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [COMBOEND]"), GetNetId());
	check(GetNetStatus() == NET_SLAVE);

	Super::ReceiveComboEnd(static_cast<EAttackState>(InLastAttackState), IsAttackCanceled);
}

void ABladeIINetPlayer::RemoteTakeDamage(float DamageAmount, uint32 DamageInfoHash, uint32 DamageCauserNetID, int32 VictimHealth, int32 ReceivedDamageNum)
{
	//if (Role == ROLE_SimulatedProxy)
	//{
	//	APawn* DamageCauser = GetNetGameMode()->FindPawnByNetId(DamageCauserNetID);
	//	const FDamageInfo* DamageInfo = FDamageInfoContainer::GetDamageInfo(DamageInfoHash);
	//	if (DamageInfo == nullptr)
	//		DamageInfo = FDamageInfo::GetDefaultDamagePtr();

	//	FDamageEvent DmgEvent;
	//	DmgEvent.DamageInfo = DamageInfo;
	//	if (DamageCauser != nullptr)
	//	{
	//		ReadyToTakeDamage(true);
	//		TakeDamage(DamageAmount, DmgEvent, nullptr, DamageCauser);
	//	}

	//	RemoteAdjustHealth(VictimHealth, DamageCauserNetID);
	//}
}

void ABladeIINetPlayer::RemoteRequestDamage(float DamageAmount, uint32 DamageInfoHash, uint32 DamageCauserNetID)
{
	/*if (Role == ROLE_Authority)
	{
		ABladeIICharacter* DamageCauser = Cast<ABladeIICharacter>(GetNetGameMode()->FindPawnByNetId(DamageCauserNetID));
		const FDamageInfo* DamageInfo = FDamageInfoContainer::GetDamageInfo(DamageInfoHash);
		if (DamageInfo == nullptr)
			DamageInfo = FDamageInfo::GetDefaultDamagePtr();
		if (DamageCauser != nullptr)
			RequestDamage(DamageAmount, DamageInfo, DamageCauser, false);
	}*/
}

void ABladeIINetPlayer::RemoteAdjustHealth(float health, uint32 DamageCauserNetId)
{
	UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [HEALTH] %f"), GetNetId(), health);

	if (bTemporarilyNetOff)
		return;

	auto KillingDamage = FMath::Abs(Health - health);
	SetHealth(health, true);
	
	GetNetGameMode()->HandleHealth(NetId, health, DamageCauserNetId);
}

void ABladeIINetPlayer::RemoteDie(uint32 OwnerNetId, uint32 KillerNetId, EPCClass KilledClass)
{
	/*if (Role == ROLE_SimulatedProxy && KilledClass == GetCurrentPlayerClass())
	{
		GotRemoteDyingSignal = true;
		GetNetGameMode()->HandleCharacterDead(this, KillerNetId, Health);
		GotRemoteDyingSignal = false;
	}*/
}

void ABladeIINetPlayer::RemoteUpdateTarget(uint32 TargetId, float Dist)
{
	auto* Target = Cast<ABladeIICharacter>(GetNetGameMode()->FindPawnByNetId(TargetId));

	auto* PrevActor = TargetActor;
	TargetActor = Target;

	if (TargetActor == NULL || PrevActor != TargetActor || Dist > MaxDashDistance)
	{
		StopDash();
	}
}

// void ABladeIINetPlayer::RemoteCounterAttack(FVector pos, FRotator rot)
// {
// 	check(GetNetStatus() == NET_SLAVE);
// 	SyncLocationAndRotation(pos, rot);
// 
// 	bForceCounterAttack = true;
// }

void ABladeIINetPlayer::RemoteResurrect(FVector pos, FRotator rot, TMap<EResurrectBuffType, uint8>& BuffInfos)
{
	//UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Receive [RESURRECT]"), GetNetId());
	check(GetNetStatus() == NET_SLAVE);
	SyncLocationAndRotation(pos, rot);
	
	//GetSkillRTManager()->BornAgain(false);
}

void ABladeIINetPlayer::RemoteEtherActivated(uint32 EtherOwnerNetId, uint32 EtherCauserNetId, int32 EtherSetID)
{
	////if (EtherOwnerNetId == GetNetId() && GetEtherContainer())	// EtherOwner가 this라면 ( Authority/simulated 상관없음 )
	//{
	//	if (UB2EtherSetEffect* EtherObject = GetEtherContainer()->GetEtherSetEffect(EtherSetID))
	//	{
	//		// ex - Offense : Thunder Ether [ EtherCauser == LocalPawn(Authority), EtherOwner == this(Simulated) ] 가 된다.
	//		// ex - Defense : Burning Ether [ EtherCauser == this(Simulated), EtherOwner == LocalPawn(Authority) ] 가 된다.
	//		 
	//		ABladeIICharacter* EtherCauser = Cast<ABladeIICharacter>(GetNetGameMode()->FindPawnByNetId(EtherCauserNetId));
	//		EtherObject->ActivateEther(EtherCauser);
	//	}
	//}
}

void ABladeIINetPlayer::RemoteAddBuff(uint32 BuffOwnerNetId, uint32 BuffCauserNetId, EBuffType BuffType, float fDuration, float Amount, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams)
{
/*	if (BuffOwnerNetId == GetNetId())
	{
		ABladeIICharacter* BuffCauser = Cast<ABladeIICharacter>(GetNetGameMode()->FindPawnByNetId(BuffCauserNetId));

		Super::AddBuff(BuffType, fDuration, Amount, BuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);
	}*/	
}


void ABladeIINetPlayer::RemoteSetAnimState(uint32 netId, const FString& ReceivedState)
{
	//if (GetNetId() == netId && Role == ROLE_SimulatedProxy)
	//{
	//	if (auto* AnimInstance = GetAnimInstance())
	//		AnimInstance->SetReplicatedState(ReceivedState);
	//}
}

void ABladeIINetPlayer::RemoteSetDamageNum(uint32 netId, int32 ReceivedDamageNum)
{
	//if (GetNetId() == netId && Role == ROLE_SimulatedProxy)
	//	DamagedNum = ReceivedDamageNum;
}

EPCClass ABladeIINetPlayer::GetTagPlayerClass()
{
	return /*CachedSkillRTManager ? CachedSkillRTManager->GetTagPlayerClass() : */EPCClass::EPC_End;
}

bool ABladeIINetPlayer::GetIsLocalPlayerTeam()
{
	bool bIsMine = false;
	ABladeIINetPlayer* pLocalPawn = Cast<ABladeIINetPlayer>(GetNetGameMode()->GetLocalController()->GetPawn());

	if (pLocalPawn)
	{
		bIsMine = (!IsEnemy(pLocalPawn->GetTeamNum()));
	}

	return bIsMine;
}

void ABladeIINetPlayer::SetMovementDisable(bool bSet)
{
	auto* MyPawnMovement = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MyPawnMovement)
	{
		MyPawnMovement->SetMovementMode( bSet ? MOVE_None : MOVE_Falling);
	}
}

void ABladeIINetPlayer::SetNetId(uint32 id)
{
	NetId = id;
	SendAnimStateIndex();
}

ESkillAnimType ABladeIINetPlayer::GetCurrentSKillAnimIdx_Native()
{
	if (IsRemoteCharacter())
		return m_SkillAnimTypeForRemotePlayer;
	else
		return Super::GetCurrentSKillAnimIdx_Native();
}

void ABladeIINetPlayer::UpdateUIDocOnTargetDamaged()
{
	if (GetNetStatus() == NET_SLAVE)
		return;

	Super::UpdateUIDocOnTargetDamaged();
}

int32 ABladeIINetPlayer::GetCurCharacterLevel()
{
	check(GetNetStatus() == NET_SLAVE);
	return this->CharacterLevel;
}

float ABladeIINetPlayer::GetResurrectionTimerRate()
{
	if (ResurrectionZoneActor)
		return ResurrectionZoneActor->GetResurrectionTimeRate();

	return 0.0f;
}

void ABladeIINetPlayer::NotifyTagSpawn(class ABladeIIPlayer* OldRetiringPlayer)
{
	auto  Asset = GetInjectedGameRule()->GetBehaviorTreeAsset();
	if (!Asset.IsEmpty())
	{
		BehaviorTree = LoadObject<UBehaviorTree>(NULL, *Asset);
	}	

	bTemporarilyNetOff = true;

	Super::NotifyTagSpawn(OldRetiringPlayer);
}

void ABladeIINetPlayer::OnSpawnEnd()
{
	//// 리모트 캐릭터는 노티파이가 확률로 들어온다. 로컬이 채널통해서 알려주자.
	//if (GetNetStatus() == NET_MASTER || GetNetStatus() == NET_AIMASTER)
	//{
	//	FString encoded_string = ToMessageHelper::SpawnEndMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//	SendMessage(encoded_string);

	//	bTemporarilyNetOff = false;
	//}
}

void ABladeIINetPlayer::RemoteOnSpawnEnd()
{
	bTemporarilyNetOff = false;
}

void ABladeIINetPlayer::NotifyBornAgain(class ABladeIIPlayer* PrevLife)
{
	if (ABladeIINetPlayer* PrevNetPlayer = Cast<ABladeIINetPlayer>(PrevLife))
		ResurrectionCount = PrevNetPlayer->GetResurrectionCount() + 1;

	//// NetGameMode 모드마다 ABladeIIPlayer*를 가지고있을 경우 이 이벤트로 변경을 알려준다.
	//BornAgainChangedPlayerClass<int32, ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Signal(GetNetId(), PrevLife, this);

	//if (SpawnResurrectionZoneTimer.IsValid())
	//	GetWorldTimerManager().ClearTimer(SpawnResurrectionZoneTimer);

	//DestroyResurrectionZoneActor();

	//if (GetNetStatus() == NET_SLAVE)
	//	return;

	//BII_CHECK(CachedSkillRTManager);

	//FString encoded_string = ToMessageHelper::ResurrectMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//SendMessage(encoded_string);

	////AI면 켜준다.
	//if (GetNetStatus() == NET_AIMASTER)
	//{
	//	auto* MyController = Cast<ABladeIIPlayerAIController>(GetController());

	//	if (MyController)
	//		MyController->BeginAutoPlay();
	//}
	//else
	//{ // 아닌 경우도 Auto 를 켰을 수 있으므로 물려줌.
	//	TakeOverAutoFromPredecessor(PrevLife);
	//}

	//SetTeamNum(PrevLife->GetTeamNum());

	//// RAID AspectRatioAdaptiveFOV, 20180716
	//// 특정 모드에서의 전투맵 디자인 사정으로 AspectRatioAdaptiveFOV 를 사용하는 경우가 있음.
	//if (PrevLife->TopDownCameraComponent && PrevLife->TopDownCameraComponent->GetAspectRatioAdaptiveFOV())
	//{
	//	if (TopDownCameraComponent)
	//	{
	//		TopDownCameraComponent->SetAspectRatioAdaptiveFOV(true);
	//	}
	//}

	////UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [RESURRECT]"), GetNetId());
}

int64 ABladeIINetPlayer::GetAccountId()
{
	if (ICharacterDataStore* CharDataStore = GetCharacterDataStore())
		return CharDataStore->GetAccountId();

	return 0;
}

void ABladeIINetPlayer::SetDamagedNum(int32 NewDamageNum)
{
	Super::SetDamagedNum(NewDamageNum);

	/*if (Role == ROLE_Authority && NewDamageNum != OldDamagedNum)
	{
		packet::ByteStream payload;

		payload << GetDamagedNum();

		FString encoded_string = packet::FinalizePacket(packet::CHANGE_DAMAGENUM, GetNetId(), 0, packet::ALLBUTME, payload);
		SendMessage(encoded_string);
		UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [SetDamagedNum] DamageNum : %d"), GetNetId(), NewDamageNum);

		OldDamagedNum = NewDamageNum;
	}*/
}

// void ABladeIINetPlayer::StartAttackFromInput()
// {
// 	Super::StartAttackFromInput();
// }
// 
// void ABladeIINetPlayer::StopAttackFromInput()
// {
// 	Super::StopAttackFromInput();
// }

// void ABladeIINetPlayer::StartGuardFromInput()
// {
// 	if (m_bGuardKeyPressed)
// 		return;
// 
// 	Super::StartGuardFromInput();
// 
// 	if (GetAttackState() == EAttackState::ECS_Evade || GetAttackState() == EAttackState::ECS_GuardStart || bSkillCancel)
// 	{
// 		FString encoded_string = ToMessageHelper::GuardMessage(this, GetNetId(), 0, packet::ALLBUTME, bSkillCancel, bNeedUpdateTarget, static_cast<uint8>(LastGuardBreakType), static_cast<uint8>(GetAttackState()));
// 		SendMessage(encoded_string);
// 
// 		bSentGuardPacket = true;
// 	}
// }

// void ABladeIINetPlayer::StopGuardFromInput()
// {
// 	Super::StopGuardFromInput();
// 
// 	if (bSentGuardPacket)
// 	{
// 		FString encoded_string = ToMessageHelper::StopGuardMessage(this, GetNetId(), 0, packet::ALLBUTME);
// 		SendMessage(encoded_string);
// 
// 		bSentGuardPacket = false;
// 	}
// }

void ABladeIINetPlayer::RotateToTarget(float DeltaTime, bool ForceRotateForLowFPS /*= false*/)
{
	// 슬레이브는 회전로직 무시. 넷싱크로 카바해본다.(AI제외)
	if (!IsAIControlled() && GetNetStatus() == NET_SLAVE)
		return;

	if(CanRotateToTarget())
		Super::RotateToTarget(DeltaTime, ForceRotateForLowFPS);
}

void ABladeIINetPlayer::NetSendUpdateLocation(bool bForceSkip /*= false*/)
{
	if (GetNetStatus() == NET_SLAVE)
		return;

	////  	if (bForceSkip || (ClientRootMotionParams.bHasRootMotion && NetMessageLOD == 1))
	////  	{
	//// 		FString encoded_string = ToMessageHelper::RotationMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//// 		SendMessage(encoded_string);
	////  		return;
	////  	}

	//auto velocity = GetVelocity();
	//auto location = GetLocation();
	//auto rotation = GetActorRotation();

	//if ((Utilities::IsNearlyEqual(OldLocation, location) && Utilities::IsNearlyEqual(OldRotation, rotation) && Utilities::IsNearlyEqual(OldVelocity, velocity)) && NetMessageLOD >= 2)
	//	return;

	//FString encoded_string = ToMessageHelper::MoveMessage(this, GetNetId(), 0, packet::ALLBUTME);
	//SendMessage(encoded_string);
}

/*
void ABladeIINetPlayer::NetSendUpdateAnimation(bool bForce // = false)
{
	if (GetNetStatus() == NET_SLAVE)
		return;

	if (auto* AnimInst = GetAnimInstance())
	{
		FString encoded_string = ToMessageHelper::AnimationMessage(this, GetNetId(), 0, packet::ALLBUTME, AnimInst);
		SendMessage(encoded_string);
	}
}
*/

void ABladeIINetPlayer::SendMessage2(FString const& message)
{
	//GetNetGameMode()->SendMessage(message);
}

bool ABladeIINetPlayer::IsVisibleFloorRing()
{
	return IsAllyPlayer();
}

bool ABladeIINetPlayer::IsAllyPlayer()
{
	//AB2NetGameMode* GameMode = GetNetGameMode();

	//auto LocalPlayerTeamNum = -1;

	//if (GetB2GameModeType(this) == EB2GameMode::Raid)
	//{
		return true;
	//}
	//else
	//{
	//	if (GameMode)
	//		LocalPlayerTeamNum = GameMode->GetLocalPlayerTeamNum();
	//}

	//return (LocalPlayerTeamNum != -1 && LocalPlayerTeamNum == GetTeamNum());

}

void ABladeIINetPlayer::SpawnHelpResurrectionZone()
{
	if (SpawnResurrectionZoneTimer.IsValid())
		GetWorldTimerManager().ClearTimer(SpawnResurrectionZoneTimer);

	DestroyResurrectionZoneActor();

	//ResurrectionZoneActor = AB2ResurrectionZone::SpawnHelpAreaActor(ResurrectionZoneClass, this);
}

FVector ABladeIINetPlayer::GetNetAdjustGoalLocation()
{
	return NetAdjustGoalLocation + (GetVelocity() * (GetWorld()->GetTimeSeconds() - NetAdjustGoalLocationTime));
}

void ABladeIINetPlayer::DestroyResurrectionZoneActor()
{
	if (ResurrectionZoneActor)
	{
		ResurrectionZoneActor->Destroy();
		ResurrectionZoneActor = nullptr;
	}
}

int32 ABladeIINetPlayer::GetGameEntryID() const
{// 게임 레벨 안에서 본 캐릭터를 식별 가능하게 할 ID 가 됨. NetID 를 반영한다.
	// 사용자가 조작하는 로컬 플레이어 캐릭터라면 NetId 를 반영하지 않는데 NetId 가 없는 상황에서도 식별이 필요한 경우가 있어서.
	return IsLocalPlayer() ? PCClassToInt(GetCurrentPlayerClass()) : ((NetId * 10) + PCClassToInt(GetCurrentPlayerClass()));
}
bool ABladeIINetPlayer::HasValidGameEntryID() const
{
	return IsLocalPlayer() || (NetId != 0); // 원격 캐릭터는 아직 NetId 를 부여받지 못한 상황에서는 GameEntryID 를 쓰면 안됨.
}

void ABladeIINetPlayer::SyncLocationAndRotation(FVector pos, FRotator rot)
{
	check(GetNetStatus() == NET_SLAVE);

	auto location = GetActorLocation();
	auto rotation = GetActorRotation();

	if (location != pos || rotation != rot)
		TeleportTo(pos, rot);
}

void ABladeIINetPlayer::PreChangeMoveAttackState(EAutoMoveAttackState & InAutoMoveAttack)
{
	if (InAutoMoveAttack == ABladeIIPlayer::AutoMoveToTarget)
		InAutoMoveAttack = ABladeIIPlayer::NoSearchTarget;
}


bool ABladeIINetPlayer::CanHandlePacket(uint32 PacketOwner) const
{
	return !Isolated && PacketOwner == GetNetId();
}

uint32 ABladeIINetPlayer::GetNetIdFrom(AActor* A) const 
{
	auto* NetChar = Cast<ABladeIINetCharacter>(A);

	if(NetChar)
		return NetChar->GetNetId();

	auto* NetPlayer = Cast<ABladeIINetPlayer>(A);

	if(NetPlayer)
		return NetPlayer->GetNetId();

	return 0;
}

AB2NetGameMode*	ABladeIINetPlayer::GetNetGameMode() const
{
	auto* GameMode = Cast<AB2NetGameMode>(GetGameMode());
	check(GetGameMode());

	return GameMode;
}

void ABladeIINetPlayer::SetSyncedAnimInfo(FNetSyncAnimInfo InInfo)
{
	if (IntToPCClass(InInfo.TargetPCClass) == GetCurrentPlayerClass())
	{
		if (SyncedAnimInfos.Num() > MAXSYNCANIM)
		{
			SyncedAnimInfos.Empty();
			UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::SetSyncedAnimInfo, SyncedAnimInfos.Empty"));
		}

		SyncedAnimInfos.Add(InInfo);
		UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::SetSyncedAnimInfo, Name = %s, NetID = %d, Class = %d, AttackState = %d, PCSpawnMotionState = %d, Num = %d"), 
			*GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), InInfo.AttackState, InInfo.PCSpawnMotionState, SyncedAnimInfos.Num());
	}
}

bool ABladeIINetPlayer::GetSyncedAnimInfo(FNetSyncAnimInfo& OutInfo)
{
	bool IsAnim = false;
	if (SyncedAnimInfos.IsValidIndex(0))
	{
		IsAnim = true;
		OutInfo = SyncedAnimInfos[0];
		SyncedAnimInfos.RemoveAt(0);
		UE_LOG(LogTemp, Log, TEXT("ABladeIINetPlayer::GetSyncedAnimInfo, Name = %s, Netid = %d, Class = %d, AttackState = %d, PCSpawnMotionState = %d, Num = %d"), 
			*GetName(), GetNetId(), PCClassToInt(GetCurrentPlayerClass()), OutInfo.AttackState, OutInfo.PCSpawnMotionState, SyncedAnimInfos.Num());
	}
	return IsAnim;
}

bool ABladeIINetPlayer::IsAIControlled()
{
	ABladeIIPlayerAIController* pController = Cast<ABladeIIPlayerAIController>(GetController());

	if (pController)
		return true;

	return false;
}

void ABladeIINetPlayer::AdjustRootMotionBlock()
{
	//EAttackState CurrentAttackState = GetAnimInstance()->GetAnimAttackState();

	//// 체크케이스 - 반격이 문제니 반격들만 체크함
	//bool bNeedCheck = CurrentAttackState == EAttackState::ECS_Counter01
	//	|| CurrentAttackState == EAttackState::ECS_Counter01Dash
	//	|| CurrentAttackState == EAttackState::ECS_Counter02
	//	|| CurrentAttackState == EAttackState::ECS_Counter02Dash
	//	|| CurrentAttackState == EAttackState::ECS_Counter03
	//	|| CurrentAttackState == EAttackState::ECS_Counter03Dash
	//	|| CurrentAttackState == EAttackState::ECS_Counter04
	//	|| CurrentAttackState == EAttackState::ECS_Counter04Dash
	//	|| CurrentAttackState == EAttackState::ECS_Counter05
	//	|| CurrentAttackState == EAttackState::ECS_Counter05Dash;

	//if (!bNeedCheck)
	//{
	//	m_vCachedLocationForRootMotionBlock = GetActorLocation();
	//	return;
	//}

	//TArray<FHitResult> HitResults;

	//FVector vDir = GetActorForwardVector();//GetActorLocation() - m_vCachedLocationForRootMotionBlock;

	//vDir = vDir.GetSafeNormal() * 10;

	//FComponentQueryParams Params(SCENE_QUERY_STAT(MoveComponent), this);

	//GetWorld()->ComponentSweepMulti(HitResults, GetCapsuleComponent(), m_vCachedLocationForRootMotionBlock, m_vCachedLocationForRootMotionBlock + vDir, GetActorRotation(), Params);

	//bool bHadBlockingHit = false;

	//// 캐릭터한테 막힌경우만 처리
	//for (int nCnt = 0; nCnt < HitResults.Num(); ++nCnt)
	//{
	//	if (HitResults[nCnt].bBlockingHit && HitResults[nCnt].GetActor())
	//	{
	//		ABladeIICharacter* pChar = Cast<ABladeIICharacter>(HitResults[nCnt].GetActor());
	//		if (pChar)
	//		{
	//			bHadBlockingHit = true;
	//		}
	//	}
	//}

	//if (HasRootMotion() && bHadBlockingHit)
	//{
	//	SetActorLocation(m_vCachedLocationForRootMotionBlock);
	//}
	//else
	//{
	//	m_vCachedLocationForRootMotionBlock = GetActorLocation();
	//}
}

void ABladeIINetPlayer::UpdateVelocity(const FVector& vel)
{
	UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	BII_CHECK(CharMoveComp);

	CharMoveComp->Velocity = vel;
	CharMoveComp->SetMovementMode(MOVE_Walking);
	CharMoveComp->UpdateComponentVelocity();
}

void ABladeIINetPlayer::ReadyToTakeDamage(bool bReady)
{
	GotRemoteTakeDamage = bReady;
}