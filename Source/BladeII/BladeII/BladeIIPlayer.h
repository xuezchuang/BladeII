// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
// Author : leejswo(SangwooLee)
#pragma once
#include "BladeIICharacter.h"
#include "CommonStruct.h"
#include "UObject/ObjectSaveContext.h"
#include "BladeIIPlayer.generated.h"

static const float EVADE_COOL_TIME = 3.0f;
static const float QTE_ENABLE_DISTANCE = 500.0f;
static const float QTE_ENABLE_SAFE_DISTANCE = 1000.0f;
static const float RESTART_TIME = 60.0f;

UENUM(BlueprintType)
enum class ETargetingMode : uint8
{
	ETM_Normal = 0,
	ETM_RetargetNearest,
	ETM_End
};

UENUM(BlueprintType)
enum class ETargetDirection : uint8
{
	ETD_Forward = 0,
	ETD_Left,
	ETD_Right,
	ETD_Back,
	ETD_End
};

UENUM(BlueprintType)
enum class ETagAttackMode : uint8
{
	EAM_Disable = 0,
	EAM_DownAttack,
	EAM_DashAttack,
	EAM_End
};

// Need to sync with UB2SkillRTManager::GetSkillAnimIndex
UENUM(BlueprintType)
enum class ESkillAnimType : uint8
{
	ESA_Skill01_01 = 0,
	ESA_Skill01_02,
	ESA_Skill01_03,
	ESA_Skill02_01,
	ESA_Skill03_01,
	ESA_Skill03_02,
	ESA_Skill03_03,
	ESA_Skill04_01,
	ESA_Skill05_01,
	ESA_Skill05_02,
	ESA_Skill05_03,
	ESA_Skill06_01,
	ESA_Weapon_01,
	ESA_Weapon_02,
	ESA_Weapon_03,
	ESA_Weapon_04,
	ESA_Weapon_05,
	ESA_Weapon_Normal_End,
	// This is for easy blending in AnimBP, don't use this value for other cases
	ESA_Weapon_01_LOD,
	ESA_Weapon_02_LOD,
	ESA_Weapon_03_LOD,
	ESA_Weapon_04_LOD,
	ESA_Weapon_05_LOD,
	
	ESA_End
};

/** Combination of PCClass and SillAnimType. It can be unique identifier of each skill animation. */
USTRUCT()
struct FCombinedPCSkillAnimID
{
	GENERATED_USTRUCT_BODY()

	// Resembles FCombinedNPCClassID in many ways.

	FCombinedPCSkillAnimID()
	{
		PCClassEnum = EPCClass::EPC_End;
		SkilAnimType = ESkillAnimType::ESA_End;
	}
	FCombinedPCSkillAnimID(EPCClass InClassEnum, ESkillAnimType InSkillAnimType)
	{
		PCClassEnum = InClassEnum;
		SkilAnimType = InSkillAnimType;
	}
	FORCEINLINE bool operator==(const FCombinedPCSkillAnimID& InOther) const
	{
		return (this->PCClassEnum == InOther.PCClassEnum && this->SkilAnimType == InOther.SkilAnimType);
	}
private:
	EPCClass PCClassEnum;
	ESkillAnimType SkilAnimType;
public:
	FORCEINLINE EPCClass GetPCClassEnum() const { return PCClassEnum; }
	FORCEINLINE ESkillAnimType GetSkillAnimType() const { return SkilAnimType; }
	FORCEINLINE uint32 GetMyTypeHash() const { return static_cast<uint32>(PCClassEnum) * 100 + static_cast<uint32>(SkilAnimType); }
};
/** For use of TMap with FCombinedPCSkillAnimID key */
FORCEINLINE uint32 GetTypeHash(const FCombinedPCSkillAnimID& InId)
{
	return InId.GetMyTypeHash();
}

UENUM(BlueprintType)
enum class EWeaponSkillType : uint8
{
	EWS_None = 0,
	EWS_Weapon01,
	EWS_Weapon02,
	EWS_Weapon03,
	EWS_Weapon04,
	EWS_Weapon05,
	EWS_End
};

/** To play different spawn motion for each case. */
UENUM(BlueprintType)
enum class EPCSpawnMotionState : uint8
{
	// Either recently spawned by stage begin, or enough time has passed after spawn (back to this value after SpawnEnd notification)
	EPST_Normal = 0,

	// Being spawned by resurrection
	EPST_BornAgain,

	// Being spawned by resurrection for control game
	EPST_BornAgain_ForControlGame,

	// Being spawned by tagging, should match with ETagAttackMode ordering
	EPST_Tag,

	// Being spawned with Tag Down Attack
	EPST_TagAttack,

	// Being spawned with Tag Dash Attack
	EPST_TagAttack_Dash,
};

struct FCameraLengthValueForBlending
{
	FCameraLengthValueForBlending()
	{
		LimitChangeAmountPerSec = 1000;
	}
	float TargetArmLength;
	float LimitChangeAmountPerSec;
};

struct FCameraFovValueForBlending
{
	FCameraFovValueForBlending()
	{
		LimitChangeAmountPerSec = 100;
	}
	float CameraFov;
	float LimitChangeAmountPerSec;
};


#define DEFINE_AUTOMOVEATTACK_STATE_CLASS(CLASSNAME)		\
class CLASSNAME##State : public AutoMoveAttackState			\
	{														\
	public:													\
		CLASSNAME##State(ABladeIIPlayer* InPlayer)			\
			: AutoMoveAttackState(InPlayer) {}				\
		virtual void Enter() override;						\
		virtual void Leave() override;						\
		virtual void Tick(float delta) override;			\
	};														\

DECLARE_DELEGATE(FQTEOnEnd)

class ABladeIIGameMode;
class UB2UIDocBattle;

UCLASS(Blueprintable)
class ABladeIIPlayer : public ABladeIICharacter
{
	GENERATED_BODY()

protected:
	class AutoMoveAttackState
	{
	protected:
		class ABladeIIPlayer* Player;
	public:
		AutoMoveAttackState(ABladeIIPlayer* InPlayer) : Player(InPlayer)
		{}
		virtual ~AutoMoveAttackState() {}
		virtual void Enter() = 0;
		virtual void Leave() = 0;
		virtual void Tick(float delta);
		float GetCurrentTargetDistance();
		AActor* GetCurrentTargeActor();
		void StopAutoMove();
		void StartAutoMove(AActor * CurrentTarget);
		bool CheckAutoMoveRadius(float InDistance);
		bool CheckAttackTargetRadius(float InDistance);
	};

	DEFINE_AUTOMOVEATTACK_STATE_CLASS(NoSearchTarget)
	DEFINE_AUTOMOVEATTACK_STATE_CLASS(AutoMoveToTarget)
	DEFINE_AUTOMOVEATTACK_STATE_CLASS(AttackTarget)

	enum EAutoMoveAttackState
	{
		NoSearchTarget,
		AutoMoveToTarget,
		AttackTarget,
	};

	EAutoMoveAttackState CurrentMoveAttackState;

	std::shared_ptr<AutoMoveAttackState> NoSearchTargetPtr;
	std::shared_ptr<AutoMoveAttackState> AutoMoveToTargetPtr;
	std::shared_ptr<AutoMoveAttackState> AttackTargetPtr;
	std::shared_ptr<AutoMoveAttackState> CurrentAutoMoveAttackStatePtr;

	virtual std::shared_ptr<AutoMoveAttackState> GetState(EAutoMoveAttackState InAutoMoveAttack);
	virtual bool CheckChangeState(EAutoMoveAttackState InAutoMoveAttack);
	virtual void PreChangeMoveAttackState(EAutoMoveAttackState& InAutoMoveAttack);
public:
	void ChangeAutoMoveAttackState(EAutoMoveAttackState InAutoMoveAttack);
	
	bool AutoAttackMove();

	bool AutoAttack_AbleSkill();

protected:
	// For camera boom.. Might consider them as default values that the camera boom will be back after some temporary effect.
	float CameraPitch;
	float CameraYaw;
	float CameraRoll;
	float CameraDistance;

	/** Cached at the beginning to allow having other value and go back to original. */
	float PlayerTopDownCamInitialFOV;

	bool CameraBoomLockedViewTarget;

	// Dilation
	float DeltaDilation;

	/** To be picked up by AnimBP, stop all attacking motion and go back to idle pose. Will be valid for one tick. */
	uint32 bStopAttackSignal : 1;

	/** True means player almost dying.. by CriticalConditionHealthRatio */
	uint32 bIsInCriticalCondition : 1;

	/** Prevent unwanted damage while input is disabled by playing stage event */
	uint32 bInvincibleByStageEvent : 1;

	uint32 bReachedAtDeadEnd : 1;

	uint32 bbInvincibleByTutorial : 1;
		
protected:
	// Dash, moved from BladeIIPlayerController
	float TotalDashDuration;
	float CurrentDashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash)
	float MaxDashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash)
	float MaxDashSpeed;

	const float UnitySkillCoolTime = 10.0f;
	const int32 UnitySkillAddPoint = 1;
	float UnitySkillDeltaSeconds = 0;
	bool IsUnityTick;
	bool bOverUnityPoint;
	// To simulate repeat event for UMG button
	bool bKeepAttacking;

	// Cached Attack button pressed during cinematics, ugly hack
	uint32 bCachedPressedInCinematic : 1;

	// Last guard timing
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category = Combat)
	float LastGuardTime;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category = Combat)
	uint32 bForceCounterAttack : 1;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category = Combat)
	bool bReceiveNetMotionCancleSignal;

	//friend class AB2StageEventDirector;
	//friend class AB2TutorialGameMode;
	//friend class ABladeIIPlayerController;

	TArray<FQTEOnEnd> arQTEOnEndCallBack;
	
	/** It is supposed to be the same to LightComponent->bAffectsWorld in most formally expected cases.
	 * IsHeadPointLightAllowed must return true prior to this. */
	uint32 bHeadPointLightIsFormallyOn : 1;

	bool		CanEvade;

public:
	ABladeIIPlayer(const FObjectInitializer& ObjectInitializer);

	// Camera
	/** Action camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ActionCameraComponent;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/** Well, it is already exposed as public.. but be careful. Check IsHeadPointLightAllowed and FormalSetPointLightOn. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* LightComponent;

	/** How far the LightComponent above the player character mesh */
	UPROPERTY(EditAnywhere, Category = Light)
	float LightComponentAttachDistZ;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	//class UChainComponent* LeftWeapon;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	//class UChainComponent* RightWeapon;

	//UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	//class UDecalComponent* LeftFootShadow;

	//UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	//class UDecalComponent* RightFootShadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light, meta = (UIMin = 10, ClampMin = 10))
	float ShadowSizeFoot;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystemComponent* FloorRingPSC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystem* FloorRingPS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	FLinearColor FloorRingColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	float FloorRingColorMultiplier;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystemComponent* ReadyWeaponSkillPSC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystem* ReadyWeaponSkillFX;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Resurrection)
	//TSubclassOf<class AB2ResurrectionZone>	ResurrectionZoneClass;

	virtual float GetShield() const override;
	virtual void SetShield(const float NewShieldAmount) override;

	virtual bool IsComboAttacking() const override;
	virtual bool IsSkillAttacking() const override;
	virtual bool IsCounterAttacking() const override;

	virtual bool HadHugeDamaged() override;
	virtual bool IsGuarding() override;

	void SpawnResurrectBuffTextEffect();

	virtual int32 GetSkillLOD() const { return 0; }
	
	/** Auto player character cannot do the attack if current target is out of this distance. 
	 * Originally introduced to prevent some buggy attack motion, but it could be one of Auto AI configuration.
	 * Ranged class would have higher value. Different from DetectDistance or TargetingDistance anyway. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	float MaxDistanceToAllowAutoAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	class ABladeIICharacter* TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QTE)
	class ABladeIICharacter* QTEActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QTE)
	ENPCClass QTENPCClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	class AActor* TargetObject;

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override {};

	void UpdateTargetObject();
	virtual float UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled = false);
	void SetUnityTick(bool InUnityTick) { IsUnityTick = InUnityTick; }
	bool GetUnityTick() { return IsUnityTick; }
	virtual bool CanRotateToTarget();

	virtual void RotateToTarget(float DeltaTime, bool ForceRotateForLowFPS = false);
	void RotateToTarget(class AActor* InTarget);

	void UpdateCameraBoom(float DeltaTime);

	float GetDeadTimeSec();
	void SetDeadTimeSec(float fDeadTime);

	// Only in the control mode And Starting QTE situation (Not Used for QTE UI Showing)
	bool GetUseQTESafeEnebleDistance();

	class ABladeIICharacter* FindQTEEnableTarget(bool bUseSafeEnableDistance = false);

	/** Camera boom positioning the camera above the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float TargetingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bDisableRotateToTarget : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bDisableRotateToInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float CounterCheckTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float EvadeCheckTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	bool bCounterCauseStun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float RotateToTargetSpeed;

	// Reserved skill cancel, to be handled by anim BP
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bSkillCancel : 1;

	// Enable tag attack
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	ETagAttackMode TagAttackMode;

	// Camera culling for visibility
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bUseCameraCulling : 1;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float CameraCullingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	ETargetingMode TargetingMode;

	/** When player's Health to MaxHealth ratio goes below this value, it goes to critical condition. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float CriticalConditionHealthRatio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoMoveRange;

	/** The max time between successful attack to increase the combo. (Not the ECS_Combo# of EAttackState)
	 * If this is to be different among player classes, expose it editable. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	float ComboMaintainDuration;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	ETargetDirection TargetDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	float MoveToMobAcceptableRadius;

	/** To ensure setting skill cooltime timer, will be true between skill attack and next comboend notify and check tag attack triggered */
	UPROPERTY(Transient, BlueprintReadOnly)
	uint32 bLastInvokedAnySkillAttack : 1;

	// To keep Tag Attack Mode value after TagAttackNotifyState
	UPROPERTY(Transient, BlueprintReadOnly)
	uint32 bTagAttackTriggered : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directing")
	class UForceFeedbackEffect* ForceFeedbackSetting;

	/** Keep increase from 0 by successive damage attack within ComboMaintainDuration. (Not the ECS_Combo# of EAttackState) */
	int32 CurrentComboNum;

	/** The last game time executed successful attack to enemy in second */
	float LastAttackSuccessTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash)
	float BP_ConfigMinDashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash)
	float BP_ConfigMaxDashDistance;

private:
	/** Point to use Weapon Skill */
	float WeaponSkillPoint;
	FRotator TopDownCameraDefaultRotator;

	class AActor* AutoMoveTargetActor;
protected:
	ESkillAnimType CurrentSkillAnimIdx;
	int32 SkillLOD;

	UPROPERTY()
	EAttackState ReservedSkill;

	UPROPERTY()
	EAttackState CachedSkill;

	float LastSkillInputTime;

	virtual void AddWeaponSkillPoint(float ToAddPoint);
	void AttachReadyWeaponSkillFX(bool bAttach);

	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;
	virtual void ProcessTakeDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser) override;

	virtual void ProcessGuard(const FDamageInfo& DamageInfo, AActor* DamageOnPlayerBuffChangedCauser);

	virtual void PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak) override;

public:

	virtual void SetReservedSkill(EAttackState InSkillState);
	void ConsumeReservedSkill();
	EAttackState GetReservedSkill() const { return ReservedSkill; }


	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	ESkillAnimType GetCurrentSKillAnimIdx();

	virtual ESkillAnimType GetCurrentSKillAnimIdx_Native();

	virtual float GetWeaponSkillPoint() const { return WeaponSkillPoint; }
	virtual void SetWeaponSkillPoint(float ToSetPoint);
	void SetWeaponSkillPointMax();
	bool IsWeaponSkillPointMax();

	/** Adjust the camera boom's rotation pitch. Not intended for temporary camera effect when bUpdateOriginal == true . */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraPitch(float InPitch);

	/** Adjust the camera boom's rotation yaw. Not intended for temporary camera effect when bUpdateOriginal == true */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraYaw(float InYaw);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraRoll(float InRoll);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetCameraPitch() const;
		
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetCameraYaw() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetCameraRoll() const;

	/** Adjust the camera boom's TargetArmLength. Calling this during CameraDistZoom notify state can be ineffective */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraDistance(float InDistance);

	/** It directly affects top-down camera component, not the camera boom.
	 * Use this with care, as this can mess up any camera effect based on camera boom, such as CameraDistZoom. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraTransform(const FTransform& InTransform);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetCameraDistance() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraTargetFocusOffset(const FVector& Offset);
	
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	FVector GetCameraTargetFocusOffset();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateCameraBoomLockedViewTarget(bool bLockedViewTarget);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool GetCameraBoomLockedViewTarget();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void StartDash(float InDashDuration, float InMaxDistance);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void StopDash();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetTargetDistance() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	int32 GetCounterMotionIncrease()  const;

	// Force feedback only for local player
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void ForceFeedback() const;

	// Stop force feedback only for local player
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void StopForceFeedback() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool GetHadInput()  const;

	/** To be checked by AnimBP */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsTagAttackEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsKeepAttacking() { return bKeepAttacking; }

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetKeepAttacking(bool bKeep);

	// Some conditions for disable tag
	bool HadTagDisabledDamage() const;

	void SetTagEnable(bool IsEnable);
	bool CanTag();

	/** To be checked by AnimBP */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual bool CanPlayerHaveMoveMotion() const;

	/** Notification from AnimBP for DeadEnd, to stop animation, etc. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void NotifyPlayerDeadEnd();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void OnCounterStart();

	virtual void NotifyActorCustomEvent(FName EventName = NAME_None, UObject* OptionalNotifyingObject = NULL) override;
	
	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual class ABladeIICharacter*  GetNearestEnemyCharacter() override;

	virtual void PostActorCreated() override;

	virtual void GetNearestFriendCharacters(TArray<class ABladeIICharacter*>& Friends, float Range, int32 MaxNum) override;

	virtual void FilterTeamBuffAffectMember(ETeamBuffType TeamBuffType, TArray<class ABladeIICharacter*>& InChracters);

	virtual bool IsPlayerCharacter() const { return true; }
	virtual bool IsPlayerControlled() const override { return true; }

	// This is PC, not NPC. See GetCurrentPlayerClass for BladeIIPlayer
	virtual ENPCClass GetNPCClass() const override { return ENPCClass::ENC_End; }
	virtual ENPCClassVariation GetNPCClassVariation() const override { return ENPCClassVariation::ENCV_Normal; }

	void ChangeFloorRingPS(class UParticleSystem* NewFloorRingPS);
	
	void AddQTEOnEndCallBack(const FQTEOnEnd& Handler);

	virtual bool GetAbleToMove() override;

	bool IsDashAttackState(EAttackState InAttackState);

	// Unintentionally duplicated..
	FORCEINLINE bool IsLocalPlayer() const { return IsPlayerControlled() && IsLocalCharacter(); }
	virtual ABladeIICharacter* GetQTEActor() const override { return QTEActor; }

	//UFUNCTION(BlueprintCallable, Category = "Utilities|Transformation")
	virtual FVector GetVelocity() const override;

	void SetDefaultCameraRotator();

	float GetPlayerTopDownCamInitialFOV() {		return PlayerTopDownCamInitialFOV;
	}

	void UpdatePlayerBuffChanged();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual bool GetIsLocalPlayerTeam() { return IsLocalPlayer(); }

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool StartCustomAnim(UAnimSequenceBase* InAnimAsset);

	virtual void AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo) override;
	virtual void AffectToAttackerAfterVictimDie(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)override;
protected:
	/** Cached from PlayerState. */
	UPROPERTY(Transient)
	class UB2SkillRTManager* CachedSkillRTManager;

	TArray<FB2Item> CachedEquippedItem;
	FB2Wing CachedWingData; // This is valid only when bHasWing is true. Use GetWingDataPtrIfValid for safe access.
	bool bHasWing;
	FORCEINLINE const FB2Wing* GetWingDataPtrIfValid() const { return bHasWing ? &CachedWingData : NULL; }
	
	float m_fDeadTimeSec;
	void SetInAutoPlay(int32 Value);

	TArray<int32>	AutoSkillPriority;
private:

	FTimerHandle ResetComboNumTimerHandle;
	uint32 bFinalizedByPCClassInfo : 1;

	
	UPROPERTY(Transient)
	int32 InAutoPlay; // Final auto play flag. It could be temporarily false when GameMode MaintainedLocalPlayerAutoState is true

public:

	/** For class specific HUD styling, will be referred by B2InGameHUDWidget
	 * Without it, default HUD layout and style images are used.
	 * -> It was originaly intended both for player character and boss mob, but now it is just for the player character. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	//TSubclassOf<class UB2InGameHUDInfo> HUDInfoClass;
		
	/** This is not a UserWidget, but once was. We treat it like one of widget anyway, considering its role. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	class UParticleSystem* HiddenMobPointerPSTemplate;

	uint32 bCheatImmortalMode : 1; // Player won't be dead. Can be still damaged(effect, trigger, etc) unlike CheatManager's God
	uint32 bCheatHeroMode : 1; // Player kill monsters at one hit.
	virtual bool IsImmortalMode() const override;
	virtual bool IsHeroMode() const override;

	FCameraLengthValueForBlending CameraLengthValueForBlending;
	FCameraFovValueForBlending CameraFovValueForBlending;

protected:
	
	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Check for general conditions for new input, not possibly for some "stopping" input. */
	virtual bool IsFineToStartAnyNewInputHandling();
	virtual bool IsFineToStartAnyNewMoveInputHandling();

	virtual void OnStartSkill(EAttackState SkillType);

public:
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

protected:
	ETouchIndex::Type SwipeTouchIndex;
	FVector LastTouchLocation;
	uint32 bIsSwiping : 1;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual float GetMoveAcceptableRadius() override;

	void StartAttackExecute();

public: // Set Interfaces bound to user input as public, for PC auto tasks.
	virtual void StartAttackFromInput();
	virtual void StopAttackFromInput();
	virtual void StartGuardFromInput();
	virtual void StopGuardFromInput();
	
	bool IsAttackKeyPressed() { return m_bAttackKeyPressed; }
	bool IsGuardKeyPressed() { return m_bGuardKeyPressed; }
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartAnySkill();
	/** Invokes skill of player's first skill selection. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartSkill01();
	/** Invokes skill of player's second skill selection. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartSkill02();
	/** Invokes skill of player's third skill selection. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartSkill03();
	/** Invokes skill of player's weapon skill selection. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartWeaponSkill();

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

	virtual bool CanHandleInputMovement(float InputValue);
	virtual void ProcessInputMovement(float Value, float BoomYaw, EAxis::Type StandardAxis);
	virtual void OnInputMovement(const FVector& MoveDirection);

	virtual EAttackState StartSkillInner(int32 SkillInputIndex, bool IsPlayerInput);
#if !UE_BUILD_SHIPPING
	void DevTestStartWeaponSkill(ESkillAnimType WantedAnimType);
#endif

	virtual bool IsAvailableStartSkill(EAttackState InAttackState);
	virtual int32 GetHighPrioritySkillIndex();

	void StopRepeatingInput(bool bFromInput = false);

	virtual void ResetSkillCooltime();

public:
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartTag();

	virtual	void StartTagCommon(); // Common part for StartTag and ExecutePendingTag

	uint32 bIsTagPossible : 1;

	/** Temporarily set to true when tried tag while in attack state (UB2SkillRTManager::IsTagPossibleButDeferred) */
	uint32 bPendingTag : 1;
	void ExecutePendingTag();
public:


	bool InGuardBreak();
	bool IsGuardStartable();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartGuard(bool bFromInput = false);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StopGuard(bool bForce = false);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartQTEByKeyPress();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartQTE(bool bForce = false);

	virtual void StopQTEMount();

	/** Total QTE state setup method. */
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void SetQTEState() override;
	/** Simply sets bInQTEState flag (and its relevant UIDoc field) */
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void SetQTEStateFlag(bool bInState) override;
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ForcedChangeQTEStepMount();

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void OnQTENext() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnQTEAttackEnd();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool GetQTEInputEnableReady() const;

	UFUNCTION(BlueprintCallable, Category = "QTE")
	bool IsQTEMounting() const;
	bool IsQTEMountingAndReadyForAttack() const;
	
	bool IsTimingForSuccessiveQTE() const;

	virtual void OnEvadeEnd();

	virtual void OnQTEBegin();

	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetAttackState(EAttackState InAttackState) override;

protected:
	uint32 bPendingQTENext : 1;
	float QTEMountRemainingTime; // Counted while in mounted state.

	// Very final step of QTE tragedy.
	void FinalKillQTETarget();

	void ResetAllQTEUIDocState();

	/** Mainly intended to adjust projectile shooting pitch */
	virtual FVector GetDirectionToTargetCharacter();
	virtual FVector GetDirectionToTargetCharacter(FVector& BoneLocation);

	/**
	 * A little hacky feature that modifying basic Animation Blueprint functionality.
	 * It is provided to allow play wanted animation at specific state of Animation Blueprint, where the wanted animation is not placed at AnimBP.
	 * It is being called on AnimBP state transition,
	 * then check current desired state by game logic and return valid AnimSequence to override default defined AnimBP state AnimSequence.
	 */
	virtual class UAnimSequenceBase* GetAnimStateOverrideSequence() /*override*/;

	bool m_bAttackKeyPressed;
	bool m_bGuardKeyPressed;

public:
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** IsInAutoPlay "right now", You may want to check GetMaintainedLocalPlayerAutoState of BladeIIGameMode for overall auto play mode. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 IsInAutoPlay() const;

	/** Returns controller for this actor. */
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//class AB2AutoAIController* GetAutoAIController() const;

	/** Direct Start/Stop AutoPlay of Player Character. Direct use of these interfaces could be temporary purpose. 
	 * You may want to check GameMode PlayerAutoState interfaces for more formal auto state on/off.*/
	void StartAutoPlay();
	void StopAutoPlay();
	
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EnableInput(APlayerController*) override;
	virtual void DisableInput(APlayerController*) override;
	virtual void Destroyed() override;
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	uint32 bEventsSubscribed : 1; // To un-subscribe only in subscribed BladeIIPlayer object.
	virtual	void SubscribeEvents();
	virtual void UnsubscribeEvents();
	
public:

	/** Being called from GameMode, between player character spawn and post-spawn initialize.
	 * It is to ensure safe applying of PCClassInfo by GameMode routine.
	 * Be careful when you need to call it manually.
	 * bFinalizeWithoutInGameInfo : Intended for dummy puppet spawning. InGameOnlyPCClassInfo won't be used. */
	void FinalizePCClassInfoSpawn(bool bFinalizeWithoutInGameInfo = false);

	/** Initialize combat variables such as attack, defense, health, etc, based on character's level. */
	virtual void InitializeCombatStats() override;
protected:
	virtual void InitializeCombatOptionCacheValues(); // Part of InitializeCombatStats.
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Tick_InputAndMovement(float DeltaSeconds);
	virtual void Tick_QTE(float DeltaSeconds);
	
	/** Overridden to turn off the point light attached above the player's head */
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	
	/** It is rather allow per player character type.. and GameMode rule has its own allow rule. */
	virtual bool IsHeadPointLightAllowed() const { return true; }
	/** Use this to turn On/Off dynamic point light on player head, instead of direct accessing LightComponent,
	 * except for special case. */
	void FormalSetPointLightOn(bool bIsOn);
	FORCEINLINE bool IsHeadPointLightFormallyOn() const { return bHeadPointLightIsFormallyOn; }

	void SetupLightComponentAttachment();
	void UpdateLightComponentAttachment();

	/** Notified from ComboEnd anim notify.
	 * REMARK: As of initial implementation (at 2015.11), this is for player character only. To Extend it to BladeIICharacter check the AnimBP too.. 
	 * In addition, InLastAttackState is not ensured to be the same as last combo attack. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled = false);

	void OnProcessComboEnd();

	/** Returns True if the pawn can die in the current state */
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;
	
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser) override;

	virtual int64 GetAccountId();

	bool IsQTEStartAble();

protected:
	/** Apply PCClassInfo. You may optionally exclude overriding by InGameOnlyPCClassInfo (mainly for dummy puppet spawning) */
	void OverrideByPCClassInfo(class ABladeIIGameMode* InGameMode, class UB2PCClassInfo* CustomPCClassInfo = nullptr, bool bNoOverrideByInGameInfo = false);
	/** It is executed right after being spawned as ABladeIIPlayer, before overridden by PCClassInfo. */
	virtual void OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride) override;

public:
	void SetUseSectionMergedMesh(bool bUseSectionMerged);
	void DeferredSetUseSectionMergedMesh(bool bUseSectionMerged);

	virtual float GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds);
private:
	void Tick_UnitySkill(float DeltaSeconds);

	FTimerHandle DeferredSetUseSectionMergedMeshTH;
public:
	// In most cases, what you need would be just the current SkeletalMesh of MeshComponent. It is provided for special use.
	class USkeletalMesh* GetMergedMesh(bool bUseSectionMerged);
protected:
	void SetupMeshesMain(); // Wrapped SetupComponentsForParts
	/** Part of finalizing spawn, creating final whole in-game mesh, considering equipped items. */
	virtual void SetupComponentsForParts(bool bMergeSections, class USkeletalMesh** OutGetBuiltResultOnly = nullptr);
public:
	/** Provided for some case that you need to temporarily setup a mesh with any desired data store.. probably for event scene.. */
	void SetupComponentsForPartsCustomDataStore(class ICharacterDataStore* InCustomDataStore, bool bMergeSections);
	void SetupComponentsForPartsCommon(const TArray<FB2Item>& EquippedItems, const FB2Wing* InWingData, bool bMergeSections, bool bForceUpdate = false, class USkeletalMesh** OutGetBuiltResultOnly = nullptr);
	/** True in most cases.. except some dummy class cases. */
	virtual bool ShouldLoadInGameOnlyInfoForPartSetup() const { return true; }
	 
	virtual bool IsVisibleFloorRing();

	void HandleComboEndForQTE();

	void CheckSendKakaoActionLog_Death();
	void CheckSendKakaoActionLog_Kill();

private:
	virtual void NotifyTargetDamaged(class ABladeIICharacter* DamagedTarget, float DamageAmount) override;

	virtual void UpdateOscillation(float DeltaSeconds) {} // Not probably for player.

public:
	/** Sync some UI doc data which can be missing for initial value. */
	virtual void InitUpdateUIDoc() override;
protected:
	virtual void ResetComboNumTimerCallback();

	virtual void UpdateUIDocOnTargetDamaged();
	/** To update some of relevant UIDoc data on every tick basis. Be careful to put variables to be updated here. */
	
	// **BuffUpdateUIDoc : Send in-game skill buff data to UI. To be called when it is necessary, instead of calling them every frame.
	virtual void CharacterBuffUpdateUIdoc() override;
public:

	void SpawnSurviveSkillEffect();

public:
	void DestroyPlayerUIManager(); // To let a little different UI widget destroy timing

protected:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual float GetScaledAttackLevel() override { return AttackLevel; }
	virtual float GetScaledDefenceLevel() override { return DefenseLevel; }

	virtual void DropItem() override 
	{
	}

	virtual void TriggerKillMonster() override
	{
	}

public:

	/** Must get the same one to the GetSkillRTManager of BladeIIPlayerController */
	virtual class UB2SkillRTManager* GetSkillRTManager();	

	//UFUNCTION(BlueprintCallable, Category = BladeIIGame)
	//class UB2InGameHUDInfo* GetHUDInfoObject();

	/** Set up matinee group skeletal mesh actor for stage event director in controlled matinee mode.
	 * Basically setting up the skeletal mesh resource with the same one for this character,
	 * but additional code work is required according to the character customization and/or equipment stuff. */
	virtual void SetupControlledMatineePuppet(class ASkeletalMeshActor* InMatineePuppet, bool bSetupLightComponent, class USkeletalMesh* OptionalSKMeshToSet = nullptr) override;
	
	/**
	 * It simply spawns other (sub if currently main, main if currently sub) player character, without possessing it by player controller or destroy current one.
	 * Most likely for cinematic purpose.
	 * Calling side need to cache the required reference and destroy manually if not necessary anymore.
	 *
	 * bPossessByAIController : The base BP is likely to have its AutoPossessAI as PlacedInWorld, so it won't get possessed by any controller if dynamically spawned.
	 *				In that case, newly spawned puppet would not be affected by gravity or other natural stuff, etc.. Set it as true if you don't want to see that.
	 */
	class ABladeIIPlayer* SpawnOtherAsPuppet(bool bPossessByAIController = true, bool bTurnOnPointLight = false);
	class ABladeIIPlayer* SpawnMyselfAsPuppet(bool bPossessByAIController = true);

	bool IsAllowToEvade() const
	{
		return CanEvade;
	}

	float GetEvadeCoolTimeProgress() { return EvadeTimer / EVADE_COOL_TIME; }

private:
	class ABladeIIPlayer* SpawnPuppetInternal(EPCClass InSpawnClass, bool bPossessByAIController = true);

	void SetEvadeTimer();

	float		EvadeTimer;

public:
	
	void SetupLODInfoForInGame(); // Sets LOD Info variables to use low LOD only
	void SetupLODInfoForCinematic(); // Sets LOD Info variables to use high LOD only
	static void SetupLODInfoForLobbyRepresentative(class ASkeletalMeshActor* InLobbySKActor);
	virtual void SetupLODInfoAsNPC() override { /* Do none here */ }
private:
	void OnBeginWeaponSkillLOD0();
	void OnEndWeaponSkillLOD0();
	void ForceFinishWeaponSkillLOD0SceneSetup();
	void CreateWeaponSkillLOD0SceneLight();
	void DestroyWeaponSkillLOD0SceneLight();
	void UpdateWeaponSkillLOD0LightComponentAttachment();

	bool bInWeaponSkillLOD0;
	FTimerHandle WeaponSkillEndHandlingForceTH;
	UPROPERTY(Transient)
	class UPointLightComponent* WeaponSkillLOD0SceneLight;
public:

	/** Wrapper to SkillRTManager's GetTagPlayerClass. It won't return correct value for the OldPlayer which is waiting for destroy at tagging moment. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual EPCClass GetTagPlayerClass();
	
	/** Class in EPCClass enum */
	virtual EPCClass GetCurrentPlayerClass() const;

	/** True if main, otherwise, sub character or not known for any reason. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool AmIMainCharacter();

	/** Called at the moment of tagging, where this callback is called on old player about to retire. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void NotifyTagRetire(class ABladeIIPlayer* NewTaggedPlayer);
	/** Called at the moment of tagging, where this callback is called on new player just spawned. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void NotifyTagSpawn(class ABladeIIPlayer* OldRetiringPlayer);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void OnSpawnEnd();
	/** Kinda similar to TagSpawn, but it is intended for resurrection. 
	 * PrevLife is supposed to be a dead player character which is about to be destroyed soon. Could be a live one according to the usage though..? */
	virtual void NotifyBornAgain(class ABladeIIPlayer* PrevLife);
protected:
	void TakeOverAutoFromPredecessor(class ABladeIIPlayer* InPredecessor);
public:

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual EPCSpawnMotionState GetSpawnMotionState();
	/** Will be called by AnimBP SpawnEnd noti. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void EndSpawnMotionState();

	/** For some of directing stuff like slow motion and haptic feedback. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnDeadAnimBegin();

	/** Called at the moment of sucking dead monster's drop, like energy ball, gold, item, all goodies.. */
	void SuckedDropItem(class AB2DropBase* InSuckedDrop);

	/** Get class code name with common prefix trimmed from CharacterType and its relevant EPCClass enum. Almost likely the expected name that we know anyway.. */
	FString GetTrimmedNameFromPCClassEnum();

	/** Get enum names list for all pc classes, not just for this player. */
	static void GetAllTrimmedNameFromPCClassEnum(TArray<FString>& OutResult);
	
	FORCEINLINE float GetTopDownCamIntialFOV() {return PlayerTopDownCamInitialFOV;}
	
	int32 GetPlayerLevel(); // Probably return the same value as CharacterLevel
private:
	void SyncPCCharacterLevelFromExpManager(); // Need to do this before calculate PC's combat stats.

	void OnSomeMobTermination(ABladeIICharacter* InMobAboutToDestroy);

protected:
	class ICharacterDataStore* CharacterDataStore;
public:
	virtual class ICharacterDataStore* GetCharacterDataStore();

public:

	/** Call it if you need to stop attack motion immediately */
	void SignalStopAttackImmediate();
	/** To be picked up by AnimBP, to stop attack motion state at any moment. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool ConsumeAndCheckStopAttackMotion();

	FORCEINLINE bool IsInCriticalCondition() { return bIsInCriticalCondition; }

	static FName LeftFootBoneName;  
	static FName RightFootBoneName;
	static FName TargetTraceName;
	static FName QTESocketName;
	static FName FloorColorParamName;
	static FName FloorColorMultiplyParamName;

	// Character stats
	virtual float GetDamageIncreaseRate()			const override;
	virtual float GetBossDamageIncreaseRate()		const override;
	virtual float GetCriticalRate()					const override;
	virtual float GetCriticalDamageRate()			const override;
	virtual float GetDamageDecreaseRate()			const override;
	virtual float GetMeleeDamageDecreaseRate()		const override { return MeleeDamageDecreaseRate; }
	virtual float GetRangeDamageDecreaseRate()		const override { return RangeDamageDecreaseRate; }
	virtual float GetBossDamageDecreaseRate()		const override { return BossDamageDecreaseRate; }
	virtual float GetResistCriticalRate()			const override { return ResistCriticalRate; }
	virtual float GetHealHPAmountRateOnKill()		const override { return HealHPAmountRateOnKill; }
	virtual float GetAbsorbHPAmountRateOnAttack()	const override { return AbsorbHPAmountRateOnAttack; }
	virtual float GetIncAdditionalDamage()			const override { return IncAdditionalDamageValueByPassive; }
	virtual float GetDecAdditionalDamage()			const override { return DecAdditionalDamageValueByPassive; }
	virtual float GetDurationDecreaseRate()			const override;

	virtual float GetAdditionalCriticalRateByComboNum() const; // Part of GetCriticalRate, separated as it is required by UI, too.

#if 0// B2_BUILD_LEVEL <= B2_BUILD_LEVEL_QA
	void SetIncAdditionalDamage(float value) { IncAdditionalDamageValueByPassive = value; }
	void SetDecAdditionalDamage(float value) { DecAdditionalDamageValueByPassive = value; }
#endif

	// Buff
	void SetTeamBuffMode(ETeamBuffType BuffMode, ABladeIIPlayer* Buffer, uint8 TargetingType, float Amount = 0.f, float Duration = 0.f, float Range = 0.f, int32 MaxAffectNum = 0, bool bUseEffect = false, bool bUseTextEffect = false);

	virtual float GetBonusBuffDuration() const override;

	// For 4th skill, EUniqueBuffType
	virtual float GetBonusBuffDamageDecreaseAmount() const override;
	virtual float GetBuffReflectionDamageRate() const override;
	virtual float GetBonusBuffHealHPByHit() const override;
	virtual float GetBonusBuffHealHP() const override;
	virtual float GetBonusBuffIncMoveSpeed() const override;

	virtual float GetBonusBuffReceiveDamageDecreaseRate() const override;
	virtual float GetBonusBuffAttackDamageIncreaseRate() const override;

	virtual float GetAttackDamageIncreaseRateByClass(EPCClass TargetClass) const override;
	virtual float GetReceiveDamageDecreaseRateByClass(EPCClass AttackerClass) const override;

	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual float GetDamageRadiusIncreaseRate() const override;

	virtual int32 GetBonusBuffAffectNum() const override;

	virtual bool CanBeTarget() const /*override*/;

	// Team Buff
	float GetBonusTeamBuffRate(ETeamBuffType BuffType) const;
	
protected:
	//////////////////////////////////////////////////
	// Cached united combat option values. (passive)
	float BaseDamageDecreaseRate;
	float MeleeDamageDecreaseRate;
	float RangeDamageDecreaseRate;
	float BossDamageDecreaseRate;
	float ResistCriticalRate;
	float StateDurationDecreaseRate;
	float DecAdditionalDamageValueByPassive;

	// Those below might have "active" option value of the same functionality.
	float IncDamageValueByPassive; 
	float IncNormalDamageValueByPassive;
	float IncSkillDamageValueByPassive;
	float IncDamageToBossValueByPassive;
	float IncCriticalRateValueByPassive;
	float IncCriticalDamageValueByPassive;
	float IncAdditionalDamageValueByPassive;

	// Heal/absorb HP values are healing amount rate (percent * 0.01) to max health, not the probability of healing operation itself.
	float HealHPAmountRateOnCounter;
	float HealHPAmountRateOnKill;
	float AbsorbHPAmountRateOnAttack;

	float AdditionalAttackSpeedRatebyPassive;

	float IncAttackDamageByPassiveToClass[PCCLASS_ARRAY_SIZE];
	float DecReceiveDamageByPassiveFromClass[PCCLASS_ARRAY_SIZE];
	//////////////////////////////////////////////////
public:	

	//class UB2AnimInstance* GetAnimInstance() const;
	const EAttackState GetAnimAttackState() const;
	const EAttackType GetDamageAnimType() const;

	// BaseMaxWalkSpeed (by PCClassInfo) + applied some **IncMoveSpeed options, except temporary one
	virtual float GetStaticOptionAppliedWalkSpeed() const override;

	float GetOptionStatusValue(EItemOption OptionType) const; // Consider use GetUnitedOptionStatusValue instead
	float GetUnitedOptionStatusValue(EUnitedCombatOptions OptionType) const;
	
	void AddEtherSetEffect(int32 EtherSetID, float TriggerRate, float CoolTime);
	void RemoveEtherSetEffect(EEtherSetType SetType);

	//class UB2EtherSetEffect* GetEtherSetEffect(EEtherSetType SetType);
	virtual bool ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage) override;

	//UPROPERTY(Transient)
	//class UB2EtherContainer* EtherContainer;

	//FORCEINLINE class UB2EtherContainer* GetEtherContainer() { return EtherContainer; }

	void TotalApplyAnimRateScale(); // Controls Anim speed, mainly the attack speed.

	void InjectGameMode(ABladeIIGameMode* GameMode)
	{
		InjectedGameMode = GameMode;
	}

	ABladeIIGameMode* GetGameMode() const
	{
		return InjectedGameMode;
	}

	void InjectDocBattle(UB2UIDocBattle* InDoc)
	{
		InjectedDocBattle = InDoc;
	}

	UB2UIDocBattle* GetDocBattle() const
	{
		return InjectedDocBattle;
	}

	/** Number to uniquely identify player character (including pending tag character) in in-game level. 
	 * Network game character need additional identification reflecting their side. */
	virtual int32 GetGameEntryID() const { return PCClassToInt(GetCurrentPlayerClass()); }
	/** Local player always have valid GameEntryID, network mode player might has invalid entry id at some timing.. */
	virtual bool HasValidGameEntryID() const { return true; }

	/** Forcibly Count Attack*/
	void ForceCounterAttack();

	void ReceiveNetMotionCancleSignalOn() { bReceiveNetMotionCancleSignal = true; }

	float GetLastGuardTime() { return LastGuardTime; }
	bool GetForceCounterAttack() { return bForceCounterAttack; }
	bool GetReceiveNetMotionCancleSignal() { return bReceiveNetMotionCancleSignal; }
	void SetReceiveNetMotionCancleSignal(bool Value) { bReceiveNetMotionCancleSignal = Value; }
private:
	FTimerHandle OnDeathTimeDilationTH;
	void OnDeathTimeDilationTimerCB(); // To stop OnDeathTimeDilation

	FTimerHandle ReserveQTEActorReportMyDeathToBirthplaceTimerHandle;
	void ReserveQTEActorReportMyDeathToBirthplace();

	void OnQTEMountBegin();

	void CountQTEMountTime(float InDeltaSecond);

	uint32 PlayerStartAttackTicket;
	uint32 PlayerStopAttackTicket;
	uint32 PlayerStartSkill01Ticket;
	uint32 PlayerStartSkill02Ticket;
	uint32 PlayerStartSkill03Ticket;
	uint32 PlayerStartWeaponSkillTicket;
	uint32 PlayerStartGuardTicket;
	uint32 PlayerStopGuardTicket;
	uint32 PlayerStartTagTicket;
	uint32 QTEAttackTicket;
	uint32 PleaseForgetMeTicket;

	ABladeIIGameMode*	InjectedGameMode;
	UB2UIDocBattle*		InjectedDocBattle;

public:
	void OnQTEMountTargetEnd(const FTransform& QTETransform);

	virtual void SetupDecalCompForLeftShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent) override;
	virtual void SetupDecalCompForRightShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent) override;

	// Per mesh instanced AnimNotifyState data
public:
	// For B2CameraAnim
	//UPROPERTY(Transient)
	//class UCameraAnimInst* CameraAnimPlayInstance;
	float CurrentCameraAnimTime;
	int32 RemainCameraAnims;

	// For B2CameraShake
	//UPROPERTY(Transient)
	//UCameraShake* CameraShakePlayInstance;

	// For B2CameraFOV
	float CachedFOV;

	// For B2CameraDistZoom
	uint32 bCachedForCameraDistZoomState : 1;
	float CachedOriginalTargetArmLength;
	FRotator CachedOriginalCamBoomRelativeRotation;
	FVector CachedOriginalCamBoomRelativeLocation;
	float CameraDistZoomEstimatedPlayTime;
	void TryRestoreFromCameraDistZoomNS(bool bForceRestore = false); // What's need to be done at UAnimNotifyState_B2CameraDistZoom::NotifyEnd
	
	// For B2CharCamera
	float OriginalCameraLagSpeed;
	float OriginalCameraLagMaxDist;

	// For B2CharLight
	FColor OriginalLightColor;
	float OriginalLightIntensity;
	float LightPrevColorBlend;

	// For B2PlayerDisableCollision
	FVector CachedLocation;

	// For B2RandomTarget
	FVector CurrentLocation;
	FVector DeltaPerTick;
	FVector B2RandomStartLocation;
	float B2RandomTargetDuration;

	// For B2FogControl
	FLinearColor OriginalFogColor;
	FLinearColor PrevFogColor;
	float OriginalFogDensity;
	float OriginalFogStartDistance;

	float PrevFogColorBlend;
	float PrevFogDensity;
	float PrevFogStartDistance;

	virtual class UMaterialInstanceDynamic* GetChainMaterial() override;

	// For CounterAttackDungeon
public:
	FORCEINLINE void SetEnableAttack(bool bToSet) { bEnableNormalAttack = bToSet; }
	FORCEINLINE void SetInvincibleByStageEvent(bool bInInvincible) { bInvincibleByStageEvent = bInInvincible; }

	// For Tutorial
public:
	FORCEINLINE void SetInvincibleByTutorial(bool bInInvincible) { bbInvincibleByTutorial = bInInvincible; }

protected:
	bool bEnableNormalAttack;

public:
	class ABladeIICharacter* GetRandomTarget(float TargetingRadius, int32 MaxTargetNum, FVector& TargetDirection);
	class ABladeIICharacter* RemoveRandomTarget();
	void ResetRandomTarget();
	FVector LastRandomTargetLocation;
	FVector LastRandomTargetDirection;
	TArray<class ABladeIICharacter*> RandomTargets;
	
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
public:
	static bool bHideAllPCs;
	void DevCalmDown();
#endif

	virtual void OnEnterAnimState(const FName& NewState);
	virtual void OnExitAnimState(const FName& PrevState);

	virtual void OnEnterSkillState(EAttackState SkillState);
	virtual void OnEnterSkillState_Tag(EPCClass TagClass) {}

	virtual void OnExitSkillState(EAttackState SkillState);

	virtual bool IsForceApplyHitAnimation() override;

private:
	bool CheckPlayerStopTime(float DeltaTime);
	FVector PrevPosition;
	float StayTime;
	bool bRestartLevel;

public:
	void AddGuardCoolTime(float InAddCoolTime);
	void SetGuardCoolTime(float InCoolTime);
	float GetGuardCoolTime();
	bool GetHasNotTagDefence();
	void SetHasNotTagDefence(bool InHasNotTagDefence);

private:
	void UpdateGuardCoolTime(float InDeltaSeconds);

	float GuardCoolTime;
	bool HasNotTagDefence;
}; 

const FName B2PLAYER_SHADOW_SOCKET_NAME_LF = FName(TEXT("LeftFootShadow"));
const FName B2PLAYER_SHADOW_SOCKET_NAME_RF = FName(TEXT("RightFootShadow"));
const FName B2PLAYER_ROOTBONE_NAME = FName(TEXT("RootBone"));
const FName B2PLAYER_CENTERBONE_NAME = FName(TEXT("Bip001"));
const FName B2PLAYER_MOUNT_CENTERBONE_NAME = FName(TEXT("Bip001"));