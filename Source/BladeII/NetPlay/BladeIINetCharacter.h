#pragma once
//#include "BladeII.h"
#include "BladeIICharacter.h"
#include "BladeIINetCharacter.generated.h"

class AB2NetGameMode;

UCLASS()
class ABladeIINetCharacter : public ABladeIICharacter
{
	GENERATED_BODY()

public:
	static const uint8	NET_MASTER = 1;
	static const uint8  NET_SLAVE = 2;

	ABladeIINetCharacter(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginPlay()	override;
	virtual void BeginDestroy() override;
	virtual void Destroyed()	override;
	virtual void Tick(float)	override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float, struct FDamageEvent const&, class AController*, class AActor*) override;

	// APawn interfaces...
	bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;
	bool Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) override;

	// ABladeIICharacter interfaces...
	virtual void SetSignalAttack(bool signal) override;
	virtual void HealingHPByAmount(float InHealAmount, bool bUseEffect = false, bool bUseTextEffect = false) override;

	virtual void CheckQTEEnable(ETeamType HitTeam = ETeamType::ETT_End) override;

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	virtual void SetHealth(float NewHealth, bool bReceivedFromHost = false);

	virtual class UB2Buff_Base* AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* BuffCauser, bool bUseEffect = true, bool bUseTextEffect = true, TArray<float>* PtrAdditionalParams = nullptr);

	virtual void UpdateArmorByDamageInfo(const FDamageInfo& DamageInfo) override;

	// My interfaces...
	void StartPlay();
	void SubscribeEvents();
	void UnsubscribeEvents();

	void RemoteSetAttackSignal(bool, uint32);
	void RemoteAdjustHealth(float, uint32);
	void RemoteTakeDamage(float DamageAmount, uint32 DamageInfoHash, uint32 DamageCauserNetID, int32 VictimHealth, int32 ReceivedDamageNum);
	void RemoteDie(uint32, uint32, EPCClass);
	void UpdateLocation(FVector pos, FVector vel, FRotator rot, float fCurrentSpeed);
	void AdjustLocationFromNet(float DeltaSeconds);
	void UpdateRotation(FRotator rot);
	void SyncLocationAndRotation(FVector pos, FRotator rot);

	void DemoteNetStatus();
	void PromoteNetStatus();
	
	void RemoteAddBuff(uint32 BuffOwnerNetId, uint32 BuffCauserNetId, EBuffType BuffType, float fDuration, float Amount, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams);

	void SetNetId(uint32 id)
	{
		NetId	=	id;
	}

	virtual uint32 GetNetId() const override
	{
		return NetId;
	}

	void SetNetStatus(uint8 stat)
	{
		NetStatus = stat;
	}

	uint8 GetNetStatus() const
	{
		return NetStatus;
	}

	void SetRemoteDyingSignal() { GotRemoteDyingSignal = true; }

	virtual void ApplyQTEEnable(ETeamType HitTeam = ETeamType::ETT_End);

	virtual void OnRecoverQTEBreakPoint() override;

	bool IsReservedAttackAfterSpecialAction() { return bReservedAttackAfterSpecialAction; }
	void SetReservedAttackAfterSpecialAction(bool bReserve) { bReservedAttackAfterSpecialAction = bReserve; }
protected:
	FVector GetVelocity() const;
	FVector GetLocation() const;
	void	SendMessage(FString const&);
	FVector GetNetAdjustGoalLocation();

private:
	uint32 GetNetIdFrom(AActor*) const;
	AB2NetGameMode*	GetNetGameMode() const;

	uint32			NetId;
	uint8			NetStatus;

	bool			bEventsSubscribed;
	bool			GotRemoteDyingSignal;

	uint32			ReceivedMovementTicket;
	uint32			ReceivedRotationTicket;
	uint32			ReceivedMobAttackTicket;
	uint32			ReceivedHealthTicket;
	uint32			ReceivedTakeDamageTicket;
	uint32			ReceivedDieTicket;
	uint32			ReceivedBuffActivatedTicket;

	FVector			OldLocation;
	FVector			OldVelocity;
	FRotator		OldRotation;

	int32			NetMessageLOD;
	float			NetTimer;
	float			NetFrequency;
	float			UpdateLocationRadius;

	FVector			NetAdjustGoalLocation;
	float			NetAdjustGoalLocationTime;

	bool bReservedAttackAfterSpecialAction;
};