// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "BladeIIDamageType.h"
#include "BladeIICharacter.h"
#include "Engine/StreamableManager.h"
//#include "BladeII.h"
#include "B2DamageEffectInfo.generated.h"

/** Overall behavior of stage event director actor, regarding not just camera. */
UENUM(BlueprintType)
enum class EHitSoundCase : uint8
{
	EHit_Gladiator = 0,
	EHit_Assassin,
	EHit_Wizard,
	EHit_Fighter,
	EHit_Mob,
	EHit_Boss,
	EHit_AboardMob,
	EHit_Guard,
	EHit_End
};

USTRUCT(BlueprintType)
struct FDamageNumberParams
{
	GENERATED_USTRUCT_BODY()

	FDamageNumberParams()
	{
		ColorParam = FLinearColor::Black;
		DamageNumberZOffset = 100.f;
		DamageNumberRandomOffset = 40.f;
		DigitSize.X = 30.f;
		DigitSize.Y = 40.f;
		DigitSize.Z = 0.f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor ColorParam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DigitSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageNumberZOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageNumberRandomOffset;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2DamageEffectInfo : public UObject
{
	GENERATED_BODY()

	/** If true, loaded resources won't be unloaded even on level transition. */
	static bool bRootsetOnLoad;
	static bool bAssetsRooted;

	/** Assets here are not async loading target, but still better use FStreamableManager to prevent other async loading flush.. */
	FStreamableManager InfoLoadManager;

public:
	UB2DamageEffectInfo(const FObjectInitializer& ObjectInitializer);
		
	class UParticleSystem* GetWeaponPSTCommon(TArray<TSoftObjectPtr<class UParticleSystem>>& PSTAssetArray, EDamageElement ElementType, TMap<int32, class UParticleSystem*>& InLoadedPtrMap);

	void SpawnDamageEffect(EDamageWeapon WeaponType, EDamageElement ElementType, class ABladeIICharacter* Target, EHitDirection HitDir, float RandomAngle, FRotator HitActorRot);
	void OverrideCharacterMaterial(ECharacterMaterialOverrideType MatType, class ABladeIICharacter* Target);
	void PlayHitSound(UObject* WorldContextObject, ECharacterType CharType, FVector Location, bool IsAboradMob = false);
	void PlayGuardHitSound(UObject* WorldContextObject, FVector Location);
	void PlayHitFlash(class ABladeIICharacter* Target);

	void SetupPreRenderSKMeshActorForFX(class ASkeletalMeshActor* InOutSKActorToSetup);
	void SetupPreRenderMaterialOverride(TArray<class ASkeletalMeshActor*>& InOutSkActorsToSetup);
	int32 GetDesiredPreRenderDummyNumForMtrlOverride() const;

	/** For preventing in-game loading spike, possibly for specific platform only */
	void PreloadAndCacheAllTAsset();
	void ConditionalUnloadAllOrPreserve();
private:
	bool IsEffectSpawnable(const ABladeIICharacter* Target) const;

	void UnloadAllTAssetCache();
	void ConditionalMarkLoadedAssetsRootSet();
public:
	static bool ShouldUnloadAllAssetOnLevelTransition();
	FORCEINLINE static bool AssetsAreRooted() { return bAssetsRooted; }

protected:
	// Damage Effect for Blade Weapon Type, for Each Damage Element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UParticleSystem>> BladeWeaponPST;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UParticleSystem*> LoadedBladeWeaponPST;

	// Damage Effect for Hammer Weapon Type, for Each Damage Element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UParticleSystem>> HammerWeaponPST;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UParticleSystem*> LoadedHammerWeaponPST;

	// Damage Effect for Magic Weapon Type, for Each Damage Element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UParticleSystem>> MagicWeaponPST;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UParticleSystem*> LoadedMagicWeaponPST;

	// Damage Effect for Blade Weapon Type, for Each Damage Element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UParticleSystem>> BladeWeaponPSTForPC;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UParticleSystem*> LoadedBladeWeaponPSTForPC;

	// Damage Effect for Hammer Weapon Type, for Each Damage Element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UParticleSystem>> HammerWeaponPSTForPC;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UParticleSystem*> LoadedHammerWeaponPSTForPC;

	// Damage Effect for Magic Weapon Type, for Each Damage Element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UParticleSystem>> MagicWeaponPSTForPC;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UParticleSystem*> LoadedMagicWeaponPSTForPC;

	// Damage Effect when Guarding
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> GuardPST;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		class UParticleSystem* LoadedGuardPST;

	// Damage Effect when Damage Reflection
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> ReflectDamagePST;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		class UParticleSystem* LoadedReflectDamagePST;

	// Damage Effect when Damage Reflection
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> ShieldPST;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		class UParticleSystem* LoadedShieldPST;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName GuardDamageSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ReflectDamageSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ShieldDamageSocketName;

	// Change Material for each damage element
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<class UMaterialInterface>> CharacterMaterialOverrides;
		/** Loaded cache, only for pre-load */
		UPROPERTY(Transient)
		TMap<int32, class UMaterialInterface*> LoadedCharacterMaterialOverrides;

	// Change Material for each damage number type
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDamageNumberParams> DamageNumberParams;

	// Indices match for EHitSoundCase
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class USoundCue*> HitSoundCues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bUseRandomRotation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float RandRange;

	// Hit flash params
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitFlashColorParamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> HitFlashColorParamValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> HitFlashColorParamValuesForPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitFlashIntensityParamName;
public:
	FORCEINLINE FName GetHitFlashIntensityParamName() const { return HitFlashIntensityParamName; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitFlashIntensityParamValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitFlashIntensityParamValueForPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitFlashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitFlashDurationForPC;

	// Minimum Damage effect distance to be spawned, based on 1280*720 pixel distance, diffrent aspect ratio cause difference 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageEffectRange;

	// Should be const during play
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxDamageEffectNum;

	UFUNCTION()
	void OnParticleSystemFinished(UParticleSystemComponent* FinishedComponent);

private:
	void SpawnDamageEffectInner(class UParticleSystem* Template, class ABladeIICharacter* Target, EHitDirection HitDir, float RandomAngle, FRotator HitActorRot, FName DamageSocketName);

	UPROPERTY(Transient)
	TArray<class UParticleSystemComponent*> DamageEffectComps;

	TArray<FVector2D> DamageEffectScreenPos;

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
public:
	static bool bDisableDamageEffectParticle;
	static bool bDisableDamageEffectSound;
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
public:
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};
