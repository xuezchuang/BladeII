#pragma once
//#include "BladeII.h"
#include "BladeIIPlayer.h"
#include "BladeIINetPlayer.generated.h"

class ABladeIICharacter;
class AB2NetGameMode;
class AB2ResurrectionZone;

UCLASS()
class ABladeIINetPlayer : public ABladeIIPlayer
{
	GENERATED_BODY()

public:
	static const uint8  NET_NONE		= 1;
	static const uint8	NET_MASTER		= 2;
	static const uint8  NET_SLAVE		= 3;
	static const uint8	NET_AIMASTER	= 4;

	ABladeIINetPlayer(const FObjectInitializer& ObjectInitializer);

	// AActor interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void Tick(float) override;

	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;
	virtual float TakeDamage(float,struct FDamageEvent const& ,class AController*,class AActor*) override;

	// APawn interface
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;
	virtual bool Die(float, FDamageEvent const&, AController*, AActor*) override;
	virtual void PossessedBy(AController*) override;
	//virtual bool IsPlayerControlled() const override { return NetStatus != NET_SLAVE; }

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	// ACharacter interface
	/** Apply momentum caused by damage. */
	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;
	virtual void SetClientRootMotionParams(const FRootMotionMovementParams& RMMP) ;
	
	// ABladeIICharacter interface
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser) override;
	virtual void HealingHPByAmount(float InHealAmount, bool bUseEffect = false, bool bUseTextEffect = false) override;

	virtual void SetHealth(float NewHealth, bool bReceivedFromHost = false);
	virtual bool RequestDamage(const float Damage, const FDamageInfo* DamageInfo, ABladeIICharacter* DamageCauser, bool NetBroadcast) override;

	virtual bool ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage) override;
	void RequestEtherSetEffect(uint32 InEtherCauserNetId, int32 InEtherSetID);
	//virtual class UB2Buff_Base* AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* BuffCauser, bool bUseEffect = true, bool bUseTextEffect = true, TArray<float>* PtrAdditionalParams = nullptr) override;

	// ABladeIIPlayer interface
	virtual void SubscribeEvents() override;
	virtual void UnsubscribeEvents() override;
	virtual bool CanPlayerHaveMoveMotion() const override;
	virtual bool IsFineToStartAnyNewInputHandling() override;
	virtual bool IsFineToStartAnyNewMoveInputHandling() override;

	virtual void MoveForward(float) override;
	virtual void MoveRight(float) override;

	virtual void StartAttack() override;
	virtual void StopAttack() override;
	virtual EAttackState StartSkillInner(int32 , bool) override;

	virtual void StartTagCommon() override;
	
	virtual void StartDash(float InDashDuration, float InMaxDistance) override;
	virtual void StopDash() override;

//	virtual void OnCounterStart() override;

	virtual EPCSpawnMotionState GetSpawnMotionState() override;
	virtual void EndSpawnMotionState() override;

	virtual float UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled = false) override;
	virtual void ReceiveComboEnd(EAttackState, bool) override;

	virtual void OnQTENext() override;

	//virtual UB2SkillRTManager* GetSkillRTManager() override;
	virtual class ICharacterDataStore* GetCharacterDataStore() override;

	virtual void NotifyPlayerDeadEnd() override;
	virtual void SetAttackState(EAttackState InAttackState);
	virtual void NotifyTagSpawn(class ABladeIIPlayer* OldRetiringPlayer) override;
	virtual void OnSpawnEnd() override;
	void RemoteOnSpawnEnd();
	virtual void NotifyBornAgain(class ABladeIIPlayer* PrevLife) override;
	virtual int32 GetSkillLOD() const final { return 1; } //[@AKI, 170815][M20]6己 胶懦 俺急 NetMode俊辑绰 公炼扒 LOD 窍霸 函版
	//virtual int32 GetSkillLOD() const final { return (NetStatus != NET_MASTER) ? 1 : 0; }
	virtual int64 GetAccountId() override;

	virtual bool IsHeadPointLightAllowed() const override { return false; }

	virtual void SetDamagedNum(int32 NewDamageNum) override;

// 	virtual void StartAttackFromInput();
// 	virtual void StopAttackFromInput();
//	virtual void StartGuardFromInput();
//	virtual void StopGuardFromInput();

	virtual void RotateToTarget(float DeltaTime, bool ForceRotateForLowFPS = false) override;

	// My interface
	void NetSendUpdateLocation(bool bForce = false);
	void UpdateLocation(FVector, FVector, FRotator, float);
	void AdjustLocationFromNet(float DeltaSeconds);
	void UpdateRotation(FRotator rot);
	void RemoteStartAttack(FVector, FRotator);
	void RemoteStopAttack(FVector, FRotator);
// 	void RemoteStartGuard(FVector, FRotator, uint32, uint32, uint8, uint8);
// 	void RemoteStopGuard(FVector, FRotator);
	void RemoteSkillAttack(uint8, uint8, FVector, FRotator);
	virtual void RemoteStartTag(ETagAttackMode state, EPCClass CharClass, FVector pos, FRotator rot);
	void RemoteComboEnd(uint8 InLastAttackState, bool IsAttackCanceled);
	void RemoteTakeDamage(float DamageAmount, uint32 DamageInfoHash, uint32 DamageCauserNetID, int32 VictimHealth, int32 ReceivedDamageNum);
	void RemoteRequestDamage(float DamageAmount, uint32 DamageInfoHash, uint32 DamageCauserNetID);
	void RemoteAdjustHealth(float, uint32);
	void RemoteDie(uint32, uint32, EPCClass);
	void RemoteUpdateTarget(uint32, float);
//	void RemoteCounterAttack(FVector,FRotator);
	void RemoteResurrect(FVector, FRotator, TMap<EResurrectBuffType, uint8>&);
	void RemoteEtherActivated(uint32, uint32, int32);
	void RemoteAddBuff(uint32 BuffOwnerNetId, uint32 BuffCauserNetId, EBuffType BuffType, float fDuration, float Amount, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams);
	void RemoteSetAnimState(uint32, const FString&);
	void RemoteSetDamageNum(uint32, int32);

	/** Wrapper to SkillRTManager's GetTagPlayerClass. It won't return correct value for the OldPlayer which is waiting for destroy at tagging moment. */
	virtual EPCClass GetTagPlayerClass() override;
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual bool GetIsLocalPlayerTeam() override;

	void SeEnableTakeDamageForNet(bool InStop) { EnableTakeDamageForNet = InStop; }
	void SetMovementDisable(bool bSet);

	void SetNetId(uint32 id);

	virtual ESkillAnimType GetCurrentSKillAnimIdx_Native() override;

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

	virtual void SetSpawnMotionState(EPCSpawnMotionState InState)
	{
		RemoteSpawnMotionState = InState;
	}

	bool GetEnbleTakeDamageForNet() const { return EnableTakeDamageForNet; }
	int32 GetCurCharacterLevel();

	float	GetResurrectionTimerRate();
	void	DestroyResurrectionZoneActor();
	int32	GetResurrectionCount() { return ResurrectionCount; 	}

	/** Number to uniquely identify player character (including pending tag character) in in-game level */
	virtual int32 GetGameEntryID() const override;
	virtual bool HasValidGameEntryID() const override;

	EPCClass			CurrentPlayingClass;
	EPCClass			MainPlayerClass;
	EPCClass			SubPlayerClass;

	void SetNetMessageLOD(int32 lod)
	{
		NetMessageLOD = lod;
	}

	void SetNetMessageFrequency(int32 frequency)
	{
		NetFrequency = static_cast<float>(frequency/1000.0f);
	}

	void SetOldLVR(const FVector& L, const FVector& V, const FRotator& R)
	{
		OldLocation = L;
		OldVelocity = V;
		OldRotation = R;
	}

	void SetOldRotation(const FRotator& R)
	{
		OldRotation = R;
	}

	FVector GetVelocity() const;
	FVector GetLocation() const;

	virtual void OnEnterAnimState(const FName& NewState) override;
	virtual void OnEnterSkillState_Tag(EPCClass TagClass) override;

	virtual void OnExitAnimState(const FName& PrevState) override;

	void SetLastResurrectLocation(const FVector& Loc) 
	{ 
		LastResurrectLocation = Loc; 
	}

	virtual bool IsRemoteCharacter() /*override*/;
	virtual void SendAnimStateIndex(int32 NewStateIndex) /*override*/;
	void SendAnimStateIndex();
	void ForceSendAnimStateIndex();
	void ReceiveAnimStateIndex(FNetSyncAnimInfo& NetSyncAnimInfo);
	virtual int32 GetSyncedAnimStateIndex() /*override*/;

	void ReceiveRemoteQteNext();

protected:
	virtual void UpdateUIDocOnTargetDamaged() override;
	virtual bool IsVisibleFloorRing();

	void SendMessage2(FString const&);
	bool IsAllyPlayer();
	void SpawnHelpResurrectionZone();
	FVector GetNetAdjustGoalLocation();
	void SyncLocationAndRotation(FVector, FRotator);

	virtual void PreChangeMoveAttackState(EAutoMoveAttackState& InAutoMoveAttack) override;

private:
	void CheckSendMove();
	void CheckOldVelocity();
	bool CanHandlePacket(uint32) const;
	uint32 GetNetIdFrom(AActor*) const;
	void UpdateVelocity(const FVector&);

	void ReadyToTakeDamage(bool bReady);
	
	AB2NetGameMode* GetNetGameMode() const;

	void SetSyncedAnimInfo(FNetSyncAnimInfo InInfo);
	bool GetSyncedAnimInfo(FNetSyncAnimInfo& OutInfo);

	bool IsAIControlled();

	void AdjustRootMotionBlock();

	UPROPERTY(Transient)
	ABladeIICharacter*		OldTargetActor;

	UPROPERTY(Transient)
	AB2ResurrectionZone*	ResurrectionZoneActor;

	int32				ResurrectionCount;
	uint32				NetId;
	uint8				NetStatus;

	FVector				LastResurrectLocation;
	
	bool				EnableTakeDamageForNet;
	bool				bEventsSubscribed;
	bool				GotRemoteDyingSignal;
	bool				GotRemoteTakeDamage;
	bool				Isolated;
	bool				PossessedByAI;
	int32				OldDamagedNum;
		
	FVector				OldLocation;
	FVector				OldVelocity;
	FRotator			OldRotation;

	bool				IsInvalidateInput;

	uint32				ReceivedMovementTicket;
	uint32				ReceivedRotationTicket;
	uint32				ReceivedAttackTicket;
	uint32				ReceivedStopAttackTicket;
// 	uint32				ReceivedGuardTicket;
// 	uint32				ReceivedStopGuardTicket;
	uint32				ReceivedSkillAttackTicket;
	uint32				ReceivedTagPlayerTicket;
	uint32				ReceivedComboEndTicket;
	uint32				ReceivedTakeDamageTicket;
	uint32				ReceivedRequestDamageTicket;
	uint32				ReceivedHealthTicket;	
	uint32				ReceivedUpdateTargetTicket;
//	uint32				ReceivedCounterAttackTicket;
	uint32				ReceivedEtherActivatedTicket;
	uint32				ReceivedDieTicket;
	uint32				ReceivedBuffActivatedTicket;
	uint32				ReceivedSetAnimStateTicket;
	uint32				ReceivedChangeDamageNumTicket;
	uint32				NotifyChangeMessageLODTicket;
	uint32				NotifyChangeMessageFrequencyTicket;
	uint32				ReceivedSyncAnimStateIndexTicket;
	uint32				ReceivedQteNextTicket;
	uint32				ReceivedSpawnEndTicket;
	
	EPCSpawnMotionState RemoteSpawnMotionState;

	bool				bRootMotionBegun;
	bool				bTemporarilyNetOff;

	FTimerHandle		SpawnResurrectionZoneTimer;

	int32				NetMessageLOD;
	float				NetTimer;
	float				NetFrequency;
	float				UpdateLocationRadius;

	FVector				NetAdjustGoalLocation;
	float				NetAdjustGoalLocationTime;

	bool				bSentGuardPacket;

	int32 m_nLastSendAnimStateIndex;

	ESkillAnimType m_SkillAnimTypeForRemotePlayer;

	FVector m_vCachedLocationForRootMotionBlock;

	TArray<FNetSyncAnimInfo> SyncedAnimInfos;
};