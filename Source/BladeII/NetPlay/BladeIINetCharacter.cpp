
#include "BladeIINetCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIUtil.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "B2NetGameMode.h"
#include "Event.h"
//#include "AnimNotify_B2Damage.h"
#include "BladeIINetPlayer.h"


//#include "FakeConfigure.h"

ABladeIINetCharacter::ABladeIINetCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetStatus				= NET_MASTER;
	bEventsSubscribed		= false;
	GotRemoteDyingSignal	= false;

	NetTimer				= 0.0f;
	NetFrequency			= 0.05f;
	NetMessageLOD			= 0;
	UpdateLocationRadius	= 200.0f;

	bReservedAttackAfterSpecialAction = false;
}

void ABladeIINetCharacter::BeginPlay()
{
	Super::BeginPlay();

	//if (NetStatus == NET_SLAVE)
	//	Role = ROLE_SimulatedProxy;

	//SubscribeEvents();
}

void ABladeIINetCharacter::BeginDestroy()
{
	UnsubscribeEvents();
	Super::BeginDestroy();
}

void ABladeIINetCharacter::Destroyed()
{
	UnsubscribeEvents();
	Super::Destroyed();
}

void ABladeIINetCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
//
//	if (NetStatus == NET_SLAVE && !HasRootMotion())
//	{
//		AdjustLocationFromNet(DeltaSeconds);
//		return;
//	}
//
//#if !UE_BUILD_SHIPPING
//	NetMessageLOD	= NetFakeConfigure::GetInstance().GetNetMessageLOD();
//	NetFrequency	= NetFakeConfigure::GetInstance().GetNetMessageFrequency();
//#endif
//
//	NetTimer += DeltaSeconds;
//
//	if (NetTimer < NetFrequency)
//		return;
//
//	NetTimer = 0.0f;
//
//	if (ClientRootMotionParams.bHasRootMotion && NetMessageLOD == 1)
//	{
//		auto rotation = GetActorRotation();
//
//		packet::ByteStream	payload;
//		packet::PackingHelper(payload, rotation);
//
//		FString encoded_string = packet::FinalizePacket(packet::ROTATION, GetNetId(), 0, packet::ALLBUTME, payload);
//		SendMessage(encoded_string);
//
//		OldRotation = rotation;
//
//		return;
//	}
//
//	auto velocity = GetVelocity();
//	auto location = GetLocation();
//	auto rotation = GetActorRotation();
//
//	if ((Utilities::IsNearlyEqual(OldLocation, location) && Utilities::IsNearlyEqual(OldRotation, rotation) && Utilities::IsNearlyEqual(OldVelocity, velocity)) && NetMessageLOD >= 2)
//		return;
//
//	packet::ByteStream	payload;
//	packet::PackingHelper(payload, location);
//	packet::PackingHelper(payload, velocity);
//	packet::PackingHelper(payload, rotation);
//
//	float fCurrentSpeed = 0.f;
//
//	payload << fCurrentSpeed;
//
//	FString encoded_string = packet::FinalizePacket(packet::MOVE, GetNetId(), 0, packet::ALLBUTME, payload);
//	SendMessage(encoded_string);
//
//	OldLocation = location;
//	OldVelocity = velocity;
//	OldRotation = rotation;
//
//	ClientRootMotionParams.Clear();
}

void ABladeIINetCharacter::PostInitializeComponents()
{
	AActor::PostInitializeComponents();

//	if (!IsPendingKill())
//	{
//		GetWorld()->AddPawn(this);
//
//		// update movement component's nav agent values
//		UpdateNavAgent();
//
//		if (GetMesh())
//		{
//			BaseTranslationOffset = GetMesh()->RelativeLocation;
//			BaseRotationOffset = GetMesh()->RelativeRotation.Quaternion();
//
//#if ENABLE_NAN_DIAGNOSTIC
//			if (BaseRotationOffset.ContainsNaN())
//			{
//				logOrEnsureNanError(TEXT("ACharacter::PostInitializeComponents detected NaN in BaseRotationOffset! (%s)"), *BaseRotationOffset.ToString());
//			}
//			if (Mesh->RelativeRotation.ContainsNaN())
//			{
//				logOrEnsureNanError(TEXT("ACharacter::PostInitializeComponents detected NaN in Mesh->RelativeRotation! (%s)"), *Mesh->RelativeRotation.ToString());
//			}
//#endif
//
//			// force animation tick after movement component updates
//			if (GetMesh()->PrimaryComponentTick.bCanEverTick && GetMovementComponent())
//			{
//				GetMesh()->PrimaryComponentTick.AddPrerequisite(GetMovementComponent(), GetMovementComponent()->PrimaryComponentTick);
//			}
//		}
//
//		if (GetMovementComponent() && GetCapsuleComponent())
//		{
//			GetMovementComponent()->UpdateNavAgent(*GetCapsuleComponent());
//		}
//	}
}

float ABladeIINetCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	auto*	GameMode = GetNetGameMode();

	const float	ActualDamage	= Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//const int32 FinalHealth		= GetHealth();

	//const FDamageInfo* DamageInfo	= static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
	//const uint32 DamageHash			= DamageInfo ? DamageInfo->GetDamageHash() : 0;

	//if (GetNetStatus() != NET_SLAVE && ActualDamage > 0.0f)
	//{

	//	packet::ByteStream payload;

	//	payload << GetNetId();
	//	payload << GameMode->FindNetIdByDamageCauser(EventInstigator, DamageCauser, this);
	//	payload << ActualDamage;
	//	payload << DamageHash;
	//	payload << FinalHealth;

	//	FString encoded_string = packet::FinalizePacket(packet::TAKEDAMAGE, GetNetId(), 0, packet::ALLBUTME, payload);
	//	SendMessage(encoded_string);
	//	UE_LOG(LogBladeII, Log, TEXT("NetPlayer %d - Send [TakeDamage] DamageHash : %f"), GetNetId(), DamageHash);
	//}

	return ActualDamage;
}

bool ABladeIINetCharacter::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	//if (Role == ROLE_Authority || GotRemoteDyingSignal)
	//{
	//	return Super::CanDie(KillingDamage, DamageEvent, Killer, DamageCauser);
	//}

	return false;
}

bool ABladeIINetCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	//auto*			GameMode = GetNetGameMode();
	//const bool		bDie = Super::Die(KillingDamage, DamageEvent, Killer, DamageCauser);

	//if (GotRemoteDyingSignal)
	//{
	//	// 여긴 패킷 받고 들어온거다. 호스트도.
	//	return Super::Die(KillingDamage, DamageEvent, Killer, DamageCauser);
	//}

	//if (CanDie(KillingDamage, DamageEvent, Killer, DamageCauser) && Role == ROLE_Authority)
	//{
	//	packet::ByteStream payload;

	//	const uint32 KillerNetId = GameMode->FindNetIdByDamageCauser(Killer, DamageCauser, this);
	//	const int32 KilledClass = PCClassToInt(EPCClass::EPC_End);

	//	payload << GetNetId();
	//	payload << KillerNetId;
	//	payload << KilledClass;

	//	FString encoded_string = packet::FinalizePacket(packet::DIE, GetNetId(), 0, packet::ALL, payload);
	//	SendMessage(encoded_string);
	//	UE_LOG(LogBladeII, Log, TEXT("NetCharacter %d - Send [Die] KillerNetId : %d KilledClass : %d"), GetNetId(), KillerNetId, KilledClass);
	//}

	return false;
}

void ABladeIINetCharacter::SetSignalAttack(bool signal)
{
	Super::SetSignalAttack(signal);

	if (IsInQTEBreakState() || bMovementPreventByStageEvent)
		bReservedAttackAfterSpecialAction = true;
		
	if (NetStatus == NET_SLAVE)
		return;

	if (!GetSignalAttack())
		return;

	auto location = GetLocation();
	auto rotation = GetActorRotation();

	packet::ByteStream	payload;
	packet::PackingHelper(payload, location);
	packet::PackingHelper(payload, rotation);

	payload << GetSignalAttack();
	payload << AIAttackIndex;

	FString encoded_string = packet::FinalizePacket(packet::MOB_ATTACK, GetNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage2(encoded_string);
}

void ABladeIINetCharacter::HealingHPByAmount(float InHealAmount, bool bUseEffect, bool bUseTextEffect)
{
	auto BackupHealth = Health;

	Super::HealingHPByAmount(InHealAmount, bUseEffect, bUseTextEffect);

	if (GetNetStatus() != NET_SLAVE)
	{
		packet::ByteStream	payload;
		payload << GetNetId();
		payload << Health;
		payload << GetNetId();

		FString encoded_string = packet::FinalizePacket(packet::HEALTH, NetId, 0, packet::ALLBUTME, payload);
		//SendMessage2(encoded_string);
	}

	if (GetNetStatus() == NET_SLAVE)
	{
		Health = BackupHealth;
	}
}

void ABladeIINetCharacter::CheckQTEEnable(ETeamType HitTeam)
{
	if (NetStatus == NET_SLAVE)
		return;

	Super::CheckQTEEnable(HitTeam);
}

void ABladeIINetCharacter::StartPlay()
{
	if (NetStatus == NET_SLAVE)
		return;

	SpawnDefaultController();
}

void ABladeIINetCharacter::SubscribeEvents()
{
	if (bEventsSubscribed)
		return;

	//CAPTURE_UOBJECT(ABladeIINetCharacter);

	//ReceivedMovementTicket = ReceivedMovementClass<uint32, FVector, FVector, FRotator, float>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedMovement, uint32 netid, FVector pos, FVector vel, FRotator rot, float fCurrentSpeed)
	//	if(Capture->GetNetId() != netid || Capture->NetStatus == NET_MASTER)
	//		return;
	//Capture->UpdateLocation(pos, vel, rot, fCurrentSpeed);
	//END_CAPTURE_OBJECT()
	//	);

	//ReceivedRotationTicket = ReceivedRotationClass<uint32, FRotator>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedRotation, uint32 netid, FRotator rot)
	//		if (Capture->GetNetId() != netid || Capture->NetStatus == NET_MASTER)
	//			return;
	//		Capture->UpdateRotation(rot);	
	//	END_CAPTURE_OBJECT()
	//);

	//ReceivedMobAttackTicket = ReceivedMobAttackClass<uint32, FVector, FRotator, bool, uint32>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedMobAttack, uint32 netid, FVector pos, FRotator rot, bool signal, uint32 attack_index)
	//		if (Capture->GetNetId() != netid || Capture->NetStatus == NET_MASTER)
	//			return;
	//		Capture->RemoteSetAttackSignal(signal, attack_index);
	//	END_CAPTURE_OBJECT()
	//);

	//ReceivedHealthTicket = ReceivedHealthClass<uint32, float, uint32>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedHealth, uint32 netid, float health, uint32 DamageCauserNetId)
	//		if (Capture->GetNetId() != netid || Capture->NetStatus == NET_MASTER)
	//			return;
	//		Capture->RemoteAdjustHealth(health, DamageCauserNetId);
	//	END_CAPTURE_OBJECT()
	//);

	//ReceivedTakeDamageTicket = ReceivedTakeDamageClass<uint32, uint32, float, uint32, int32, int32>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedTakeDamage, uint32 Victim_netid, uint32 Damagecauser_netid, float DamageAmount, uint32 Hash_Damageinfo, int32 Victim_Health, int32 ReceivedDamageNum)
	//		if (Capture->GetNetId() != Victim_netid || Capture->NetStatus == NET_MASTER)
	//			return;
	//		Capture->RemoteTakeDamage(DamageAmount, Hash_Damageinfo, Damagecauser_netid, Victim_Health, ReceivedDamageNum);
	//	END_CAPTURE_OBJECT()
	//);

	//ReceivedDieTicket = ReceivedDieClass<uint32, uint32, int32>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedDie, uint32 OwnerNetId, uint32 KillerNetId, int32 KilledClass)
	//		if (Capture->GetNetId() != OwnerNetId)
	//			return;
	//		Capture->RemoteDie(OwnerNetId, KillerNetId, IntToPCClass(KilledClass));
	//	END_CAPTURE_OBJECT()
	//);

	//ReceivedBuffActivatedTicket = ReceivedBuffActivatedClass<uint32, uint32, uint8, float, float, bool, bool, TArray<float>*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedBuffActivated, uint32 BuffOwnerId, uint32 BuffCauserId, uint8 BuffType, float fDuration, float fAmount, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams)
	//		if (Capture->GetNetId() != BuffOwnerId || Capture->NetStatus == NET_MASTER)
	//			return;
	//		Capture->RemoteAddBuff(BuffOwnerId, BuffCauserId, EBuffType(BuffType), fDuration, fAmount, bUseEffect, bUseTextEffect, PtrAdditionalParams);
	//	END_CAPTURE_OBJECT()
	//);

	bEventsSubscribed = true;
}

void ABladeIINetCharacter::UnsubscribeEvents()
{
	if (!bEventsSubscribed)
		return;

	ReceivedMovementClass<uint32, FVector, FVector, FRotator, float>::GetInstance().Unsubscribe(ReceivedMovementTicket);
	ReceivedRotationClass<uint32, FRotator>::GetInstance().Unsubscribe(ReceivedRotationTicket);
	ReceivedMobAttackClass<uint32, FVector, FRotator, bool, uint32>::GetInstance().Unsubscribe(ReceivedMobAttackTicket);
	ReceivedHealthClass<uint32, float, uint32>::GetInstance().Unsubscribe(ReceivedHealthTicket);
	ReceivedTakeDamageClass<uint32, uint32, float, uint32, int32, int32>::GetInstance().Unsubscribe(ReceivedTakeDamageTicket);
	ReceivedDieClass<uint32, uint32, int32>::GetInstance().Unsubscribe(ReceivedDieTicket);
	ReceivedBuffActivatedClass<uint32, uint32, uint8, float, float, bool, bool, TArray<float>*>::GetInstance().Unsubscribe(ReceivedBuffActivatedTicket);

	bEventsSubscribed = false;
}

void ABladeIINetCharacter::RemoteSetAttackSignal(bool signal, uint32 attack_index)
{
	BII_CHECK(NetStatus == NET_SLAVE);

	AIAttackIndex = attack_index;
	SetSignalAttack(signal);
}

void ABladeIINetCharacter::RemoteAdjustHealth(float health, uint32 DamageCauserNetId)
{
	SetHealth(health, true);

	if (Health > 0.0f)
	{
		GetNetGameMode()->HandleHealth(NetId, health, DamageCauserNetId);
		return;
	}
}

void ABladeIINetCharacter::RemoteTakeDamage(float DamageAmount, uint32 DamageInfoHash, uint32 DamageCauserNetID, int32 VictimHealth, int32 ReceivedDamageNum)
{
	/*if (Role == ROLE_SimulatedProxy)
	{
		APawn* DamageCauser = GetNetGameMode()->FindPawnByNetId(DamageCauserNetID);
		const FDamageInfo* DamageInfo = FDamageInfoContainer::GetDamageInfo(DamageInfoHash);
		if (DamageInfo == nullptr)
			DamageInfo = FDamageInfo::GetDefaultDamagePtr();
		
		FDamageEvent DmgEvent;
		DmgEvent.DamageInfo = DamageInfo;
		if(DamageCauser != nullptr)
			TakeDamage(DamageAmount, DmgEvent, nullptr, DamageCauser);
		RemoteAdjustHealth(VictimHealth, DamageCauserNetID);
	}*/
}

void ABladeIINetCharacter::RemoteDie(uint32 OwnerNetId, uint32 KillerNetId, EPCClass KilledClass)
{
	GotRemoteDyingSignal = true;
	GetNetGameMode()->HandleCharacterDead(this, KillerNetId, Health);
	GotRemoteDyingSignal = false;
}

void ABladeIINetCharacter::UpdateLocation(FVector pos, FVector vel, FRotator rot, float fCurrentSpeed)
{
	BII_CHECK(NetStatus == NET_SLAVE);
//
//	SetActorRotation(rot);
//
//#if !UE_BUILD_SHIPPING
//	UpdateLocationRadius = NetFakeConfigure::GetInstance().GetUpdateLocationRadius();
//#endif
//
//	if (!FVector::PointsAreNear(pos, GetActorLocation(), UpdateLocationRadius)/* || (!ClientRootMotionParams.bHasRootMotion && vel.Size() == 0)*/)
//		SetActorLocation(pos);
//
//	UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
//	BII_CHECK(CharMoveComp);
//
//	CharMoveComp->MovementMode = MOVE_Walking;
//	CharMoveComp->Velocity = vel;
//	CharMoveComp->UpdateComponentVelocity();
//
//	NetAdjustGoalLocation = pos;
//	NetAdjustGoalLocationTime = GetWorld()->GetTimeSeconds();
}

void ABladeIINetCharacter::AdjustLocationFromNet(float DeltaSeconds)
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

void ABladeIINetCharacter::UpdateRotation(FRotator rot)
{
	BII_CHECK(NetStatus == NET_SLAVE);

	SetActorRotation(rot);
}

void ABladeIINetCharacter::SyncLocationAndRotation(FVector pos, FRotator rot)
{
	BII_CHECK(NetStatus == NET_SLAVE);

	auto location = GetActorLocation();
	auto rotation = GetActorRotation();

	if (location != pos || rotation != rot)
		TeleportTo(pos, rot);
}

void ABladeIINetCharacter::DemoteNetStatus()
{
	//Role = ROLE_SimulatedProxy;
	//NetStatus = NET_SLAVE;
	//DetachFromControllerPendingDestroy();

	//// 강등되었으니 이벤트 다시붙임
	//// NetStatus먼저 바꿔야 작동한다.
	////SubscribeEvents();
}

void ABladeIINetCharacter::PromoteNetStatus()
{
	//BII_CHECK(NetStatus != NET_MASTER);

	// we should unsubscribe events... 
	// see Unsubscribe function... and then we know how this order is important...
	//UnsubscribeEvents();

	//Role = ROLE_Authority;
	//NetStatus = NET_MASTER;
	//SpawnDefaultController();
}

void ABladeIINetCharacter::RemoteAddBuff(uint32 BuffOwnerNetId, uint32 BuffCauserNetId, EBuffType BuffType, float fDuration, float Amount, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams)
{
	//if (BuffOwnerNetId == GetNetId())
	//{
	//	ABladeIICharacter* BuffCauser = Cast<ABladeIICharacter>(GetNetGameMode()->FindPawnByNetId(BuffCauserNetId));

	//	Super::AddBuff(BuffType, fDuration, Amount, BuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);
	//}
}

void ABladeIINetCharacter::ApplyQTEEnable(ETeamType HitTeam /*= ETeamType::ETT_End*/)
{
	Super::ApplyQTEEnable(HitTeam);

	if (NetStatus == NET_MASTER)
	{
		packet::ByteStream	payload;
		payload << uint8(HitTeam);

		FString encoded_string = packet::FinalizePacket(packet::QTE_ENABLE, NetId, 0, packet::ALLBUTME, payload);
		//SendMessage(encoded_string);
	}
}

void ABladeIINetCharacter::OnRecoverQTEBreakPoint()
{
	if (IsReservedAttackAfterSpecialAction())
	{
		SetReservedAttackAfterSpecialAction(false);
		SetSignalAttack(true);
	}
}

FVector ABladeIINetCharacter::GetVelocity() const
{
	return GetCharacterMovement()->Velocity;
}

FVector ABladeIINetCharacter::GetLocation() const
{
	return GetCharacterMovement()->GetActorLocation();
}

//void ABladeIINetCharacter::SendMessage(FString const& message)
//{
//	//GetNetGameMode()->SendMessage(message);
//}

FVector ABladeIINetCharacter::GetNetAdjustGoalLocation()
{
	return NetAdjustGoalLocation + (GetVelocity() * (GetWorld()->GetTimeSeconds() - NetAdjustGoalLocationTime));
}

void ABladeIINetCharacter::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (GetNetStatus() == NET_SLAVE)
		return;

	Super::ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
}

void ABladeIINetCharacter::SetHealth(float NewHealth, bool bReceivedFromHost /*= false*/)
{
	if (GetNetStatus() == NET_SLAVE && !bReceivedFromHost)
		return;

	Super::SetHealth(NewHealth, bReceivedFromHost);
}

class UB2Buff_Base* ABladeIINetCharacter::AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* BuffCauser, bool bUseEffect/* = true*/, bool bUseTextEffect/* = true*/, TArray<float>* PtrAdditionalParams/* = nullptr*/)
{
	/*if (Role == ROLE_Authority)
	{
		const uint32 CauserNetId = GetNetIdFrom(BuffCauser);

		packet::ByteStream payload;
		payload << GetNetId();
		payload << CauserNetId;
		payload << uint8(BuffType);
		payload << fDuration;
		payload << Amount;
		payload << bUseEffect;
		payload << bUseTextEffect;

		if (!PtrAdditionalParams)
			payload << int32(0);
		else
		{
			int32 nParamsCnt = (*PtrAdditionalParams).Num();
			payload << nParamsCnt;

			for (int32 nCnt = 0; nCnt < nParamsCnt; ++nCnt)
			{
				payload << (*PtrAdditionalParams)[nCnt];
			}
		}

		FString encoded_string = packet::FinalizePacket(packet::ACTIVATE_BUFF, GetNetId(), 0, packet::ALLBUTME, payload);
		SendMessage(encoded_string);

		return Super::AddBuff(BuffType, fDuration, Amount, BuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);
	}*/

	return nullptr;
}

void ABladeIINetCharacter::UpdateArmorByDamageInfo(const FDamageInfo& DamageInfo)
{
	
}

uint32 ABladeIINetCharacter::GetNetIdFrom(AActor* A) const
{
	auto* NetChar = Cast<ABladeIINetCharacter>(A);

	if (NetChar)
		return NetChar->GetNetId();

	auto* NetPlayer = Cast<ABladeIINetPlayer>(A);

	if (NetPlayer)
		return NetPlayer->GetNetId();

	return 0;
}

AB2NetGameMode*	ABladeIINetCharacter::GetNetGameMode() const
{
	auto* GameMode = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(this));
	check(GameMode);

	return GameMode;
}
