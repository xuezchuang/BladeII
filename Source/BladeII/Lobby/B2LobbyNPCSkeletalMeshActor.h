#pragma once

#include "B2LobbySkeletalMeshActorBase.h"
#include "Engine.h"
#include "B2LobbyNPCSkeletalMeshActor.generated.h"

UCLASS()
class AB2LobbyNPCSkeletalMeshActor : public AB2UnitedLobbySKMeshActorBase
{
	GENERATED_BODY()

protected:
	ELobbyNPCType MyLobbySceneType; // For any possible need..

public:
	AB2LobbyNPCSkeletalMeshActor(const FObjectInitializer& ObjectInitializer);

protected:

	/** Specially provided for smooth blended animation transition while using simple manual anim play interfaces.
	 * Without it, the animation play simply be changed from InCurrAnim to InNextAnim with no blending by PlayAnimation call
	 * It is meant to be called at tick, only during transition phase. */
	void TickForBlendedAnimTransition(class UAnimSequence* InCurrAnim, class UAnimSequence* InNextAnim, 
		float InTotalTransitionTime, // Expected to be less than the length of InCurrAnim
		float BlendWeightForNextAnim, // 0.0 ~ 1.0. 1.0 will make full InNextAnim play.
		bool bPlayingCurrAnim = true, // True means CurrAnim will be advanced while NextAnim will be staying in 0 position, false means other way, CurrAnim will be staying at last played position.
		float CurrAnimLastPos = -1.0f, // Needed only if bPlayingCurrAnim is false.
		bool bFireAnimNotifyOfCurrAnim = false,
		bool bFireAnimNotifyOfNextAnim = true // In most cases, better to respect AnimNotify of NextAnim, instead of CurrentAnim, because CurrentAnim is going away..
		);
};

/**
 * AB2LobbyDoumiGirlBase
 * Better not a boy.. a Girl.
 */
UCLASS(BlueprintType)
class AB2LobbyDoumiGirlBase : public AB2LobbyNPCSkeletalMeshActor
{
	GENERATED_BODY()

protected:
	/** The animation sequence that this actor will be playing if no other event is generated. */
	UPROPERTY(EditAnywhere)
	class UAnimSequenceBase* DefaultPose;

	FTimerHandle TempAnimPlayTH;
	float LastPlayTempAnimLength;

	FTimerHandle TempAnimPlayEndTransitionTH;
	/** Blending time from temporarily played animation to DefaultPose. */
	UPROPERTY(EditAnywhere)
	float TempAnimEndTransitionTime;
	/** Supposed to be between 0.0 ~ TempAnimEndTransitionTime */
	float CurrEndTransitionTimeCounter;

	uint32 bInTempAnimEndTransition : 1;

	UPROPERTY(Transient)
	class UAnimSequenceBase* CachedCurrPlayingTempAnim;

	////////////////////////////////////////////////
	/** Preparations for touch anim transition */
	UPROPERTY(EditAnywhere)
	uint32 bUseTouchReaction : 1;
	FTimerHandle TouchAnimTransitionTH;
	UPROPERTY(EditAnywhere)
	float TouchAnimTransitionTime;
	uint32 bInTouchAnimTransition : 1;
	/** Cached right before starting touch anim transition.. then be maintained during the transition. */
	UPROPERTY(Transient)
	class UAnimSequenceBase* CachedPreTouchAnim;
	float LastPreTouchAnimPlayedPos;
	/** Supposed to be between 0.0 ~ TouchAnimTransitionTime */
	float CurrTouchAnimTransitionTimeCounter;

	bool bCurrPlayTouchAnim;
	////////////////////////////////////////////////

private:
	/** When it is true, most anim state transition (i.e. play specific anim seq) is disabled. */
	uint32 bInAnimStateGuard : 1;
	FTimerHandle AnimStateGuardTH;

public:
	AB2LobbyDoumiGirlBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void BeginDefaultPose();

	/** Play just once and go back to DefaultPose */
	void PlayTemporaryAnim(class UAnimSequenceBase* InAnimToPlay);
private:
	void TempAnimEndTransitionCB();
	void TempAnimPlayDoneCB();
protected:
	void QuitTempAnimEndTransitionState();

	/** Event when this actor is clicked by the mouse when using the clickable interface. */
	virtual void NotifyActorOnReleased(FKey ButtonReleased = EKeys::LeftMouseButton) override;
	/** Event when this actor is under the finger when untouched when click events are enabled. */
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex) override;

	/** Main functionalities of touch reaction. Sub-classes need to provide least implementation of anim playing itself. */
	void PlayReactionOnTouch();
	void TouchAnimTransitionCB();
	void QuitTouchAnimTransitionState();
	/** Need to be overridden to call the touch anim playing itself. */
	virtual void OnPlayReactionOnTouch();
	virtual void OnTouchAnimTransitionCB();

public:
	/** Most anim play state change is being disabled for designated duration. */
	void SetAnimStateGuard(float InGuardDuration);
protected:
	void AnimStateGuardExpireCB();
	FORCEINLINE bool IsInAnimStateGuard() const { return bInAnimStateGuard; }
};

/**
 * AB2LobbyShopGirl
 * The girl standing at the summon store, help robbing money from our user.
 */
UCLASS(BlueprintType)
class AB2LobbyShopGirl : public AB2LobbyDoumiGirlBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TMap<ELobbyShopGirlState, TSoftObjectPtr<class UAnimSequenceBase>> StateAnims;
		
	class UAnimSequenceBase* GetAnimSeqOfState(ELobbyShopGirlState InState);

public:
	AB2LobbyShopGirl(const FObjectInitializer& ObjectInitializer);

	void PlayTemporaryAnimOfState(ELobbyShopGirlState InState);

	virtual void OnPlayReactionOnTouch() override;
	virtual void OnTouchAnimTransitionCB() override;
};

/**
 * AB2LobbyRelaxingDoumi
 * The girl standing at the resting point... turn our users on.. um..
 */
UCLASS(BlueprintType)
class AB2LobbyRelaxingDoumi : public AB2LobbyDoumiGirlBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<ELobbyRelaxingDoumiState, TSoftObjectPtr<class UAnimSequenceBase>> StateAnims;

	class UAnimSequenceBase* GetAnimSeqOfState(ELobbyRelaxingDoumiState InState);

public:
	AB2LobbyRelaxingDoumi(const FObjectInitializer& ObjectInitializer);

	void PlayTemporaryAnimOfState(ELobbyRelaxingDoumiState InState);

	virtual void OnPlayReactionOnTouch();
	virtual void OnTouchAnimTransitionCB();
};

UCLASS(BlueprintType)
class AB2LobbyFairyDoumi : public AB2LobbyDoumiGirlBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<ELobbyFairyDoumiState, TSoftObjectPtr<class UAnimSequenceBase>> StateAnims;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* FxParticle;
	UPROPERTY(EditAnywhere)
	FName AttachedBoneName;

	class UAnimSequenceBase* GetAnimSeqOfState(ELobbyFairyDoumiState InState);

public:
	AB2LobbyFairyDoumi(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void PlayTemporaryAnimOfState(ELobbyFairyDoumiState InState);

	virtual void OnPlayReactionOnTouch();
	virtual void OnTouchAnimTransitionCB();

	void ForcedRemoveAttachedFX();

	UFUNCTION(BlueprintCallable, Category = "FairyDoumi")
	void SetVisibleCharacter(bool bVisible);
private:
	ELobbyFairyDoumiState RandTouchState;
};

UCLASS(BlueprintType)
class AB2LobbyFlyingFairyDoumi : public AB2LobbyDoumiGirlBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		EFairyType FairyType;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* FxParticle;
	UPROPERTY(EditAnywhere)
		FName AttachedBoneName;
	UPROPERTY(EditAnywhere)
		float EffectFresnelIntensityValue;

public:
	AB2LobbyFlyingFairyDoumi(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnPlayReactionOnTouch();

	void InitLobbySceneType();
	void InitDynamicMaterial();
	void SetAttachFX(bool bAttach);

	bool IsOpenedOtherPopup();
	EFairyType GetFairyType() { return FairyType; }

	UFUNCTION(BlueprintCallable, Category = "FlyingFairyDoumi")
		void SetVisibleCharacter(bool bVisible);

private:
	class UMaterialInstanceDynamic* FairyDynamicyMaterial;

	bool IsPossibleGetFairyGift;
};

UCLASS(BlueprintType)
class AB2SealCard : public AB2LobbyDoumiGirlBase
{
	GENERATED_BODY()

public:
	AB2SealCard(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnPlayReactionOnTouch();

	UFUNCTION(BlueprintCallable, Category = "SealCard")
	void SetCardIndex(int32 value) { cardIndex = value; }

	UFUNCTION(BlueprintCallable, Category = "SealCard")
	int32 GetCardIndex() { return cardIndex; }

protected:
	UPROPERTY(EditAnywhere)
		int32 cardIndex;

private:
	class UMaterialInstanceDynamic* FairyDynamicyMaterial;

	bool IsPossibleGetFairyGift;
};