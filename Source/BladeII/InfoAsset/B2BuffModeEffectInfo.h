// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectSaveContext.h"
#include "BladeIIDamageType.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
//#include "B2FloatingAbnormalEffect.h"
#include "Engine/StreamableManager.h"
#include "../BladeII.h"
#include "B2BuffModeEffectInfo.generated.h"

USTRUCT(BlueprintType)
struct FB2MaterialScalarParamData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	FName MaterialParamName;

	/** Used this only when IntensityCurve name is NAME_None */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	float MaterialParamValue;

	FB2MaterialScalarParamData()
		: MaterialParamName(NAME_None)
		, MaterialParamValue(0.f)
	{
	}
};

USTRUCT(BlueprintType)
struct FB2MaterialParamData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	bool bUseOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	FName ColorParamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	FLinearColor ColorParamValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	TArray<FB2MaterialScalarParamData> ScalarMaterialParams;

	// Only for EBT_Hide case for now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	EDamageElement OverrideMatType;

	FB2MaterialParamData() : bUseOverride(true), OverrideMatType(EDamageElement::EDE_None)
	{
	}
};

USTRUCT(BlueprintType)
struct FB2BuffEffectData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	bool bPersistentEffect;

	//
	// UParticleSystem references here are not TAsset, and it is just because the original implementer was lazy.
	// For now, iOS memory level is not being considered as critical, and we have to get running hitch, so let's just leave it. 
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	TSoftObjectPtr<class UParticleSystem> ParticleSytem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	TSoftObjectPtr<class UParticleSystem> EndParticleSytem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	bool bAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	bool bStickGround;

	FB2BuffEffectData() : bPersistentEffect(false), bAttach(true), SocketName(NAME_None)
	{
	}

	bool MarkLoadedAssetRootSet();
};

/** To hold loaded resource reference of FB2BuffEffectData.. like FLoadedFloatingAbnormalEffectSetupInfo */
USTRUCT(BlueprintType)
struct FB2LoadedBuffEffectData
{
	GENERATED_USTRUCT_BODY()

	FB2LoadedBuffEffectData()
	{
		LoadedBasicParticleSytem = nullptr;
		LoadedEndParticleSytem = nullptr;
	}
	FB2LoadedBuffEffectData(FStreamableManager& InLoadManager, FB2BuffEffectData& InSetup, bool bLoadBasicPS = true, bool bLoadEndPS = true);

	UPROPERTY(Transient)
	class UParticleSystem* LoadedBasicParticleSytem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffEffect)
	class UParticleSystem* LoadedEndParticleSytem;

	void ClearLoaded()
	{
		LoadedBasicParticleSytem = nullptr;
		LoadedEndParticleSytem = nullptr;
	}
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2BuffModeEffectInfo : public UObject
{
	GENERATED_BODY()

	/** If true, loaded resources won't be unloaded even on level transition. */
	static bool bRootsetOnLoad;
	static bool bAssetsRooted;
	/** Assets here are not async loading target, but still better use FStreamableManager to prevent other async loading flush.. */
	FStreamableManager InfoLoadManager;

public:
	UB2BuffModeEffectInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;
#if WITH_EDITOR
	void EditorLoadAllTAsset();
#endif
	/** For preventing in-game loading spike, possibly for specific platform only */
	void PreloadAndCacheAllTAsset();
	void ConditionalUnloadAllOrPreserve();
private:
	void UnloadAllTAssetCache();
	void ConditionalMarkLoadedAssetsRootSet();
public:
	static bool ShouldUnloadAllAssetOnLevelTransition();
	FORCEINLINE static bool AssetsAreRooted() {
		return bAssetsRooted;
	}

	bool ApplyBuffMaterial(EBuffType BuffType, class ABladeIICharacter* Target);
	void ResetBuffMaterial(class ABladeIICharacter* Target);
	UParticleSystemComponent* CharacterBuffEffect(EBuffType BuffType, class ABladeIICharacter* Target, bool bEndEffect = false);

	UParticleSystem* GetOrLoadBuffEffect(EBuffType BuffType, bool IsEndEffect);

	// Material Change for PC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	TArray<FB2MaterialParamData> PCMaterialParams;

	// Material Change for Boss
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	TArray<FB2MaterialParamData> BossMaterialParams;

	// Material Change for Mob
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	TArray<FB2MaterialParamData> MobMaterialParams;

	// Material Change for InvincibleForDuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	FB2MaterialParamData InvincibleMaterialParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	TMap<EBuffType, FB2MaterialParamData> BuffMaterialParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TMap<EBuffType, FB2BuffEffectData> CharacterBuffEffects;
	/** Loaded cache */
	UPROPERTY(Transient)
	TMap<EBuffType, FB2LoadedBuffEffectData> LoadedCharacterBuffEffects;

	// Text Effects left/right offset, should be related to damage num effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect)
	float TextLeftRightOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect)
	float TextRandomOffset;

	// Other Text, currently aligned with EResurrectBuffType
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect) RESTORE_BUFF_MODE_TEXT_EFFECT_3DFX
	//TArray<class UParticleSystem*> ResurrectBuffTextPST;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect)
	//TArray<FFloatingAbnormalEffectSetupInfo> ResurrectBuffTextUIEffectSetup;
	///** Loaded cache, Key is ResurrectBuffTextUIEffectSetup array index */
	//UPROPERTY(Transient)
	//TMap<int32, FLoadedFloatingAbnormalEffectSetupInfo> LoadedResurrectBuffTextUIEffectSetup;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect)
	//FFloatingAbnormalEffectSetupInfo BreakTextUIEffectSetup;
	///** Loaded cache */
	//UPROPERTY(Transient)
	//FLoadedFloatingAbnormalEffectSetupInfo LoadedBreakTextUIEffectSetup;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect)
	//TMap<EBuffType, FFloatingAbnormalEffectSetupInfo> CharacterBuffTextUIEffectSetup;
	///** Loaded cache */
	//UPROPERTY(Transient)
	//TMap<EBuffType, FLoadedFloatingAbnormalEffectSetupInfo> LoadedCharacterBuffTextUIEffectSetup;

	// Any other stuff that cannot be included in a specific category..


	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TextEffect)
	//FFloatingAbnormalEffectSetupInfo PlayerCriticalHitTextUIEffectSetup; Critical damage effect is now within damage number widget.

	void SpawnResurrectTextEffect(EResurrectBuffType BuffType, class ABladeIICharacter* Target);
	void SpawnCharacterBuffTextEffect(EBuffType BuffType, class ABladeIICharacter* Target);
	void SpawnBreakTextEffect(class ABladeIICharacter* InTarget);

	// Additional parameter (about which kind of ciritical hit) might be added.
	//void SpawnCriticalHitTextEffect(class ABladeIICharacter* InTarget);

private:
	void SpawnTextInner(class ABladeIICharacter* Target, class UParticleSystem* PST);
	// It actually invokes DJLegacy UIManager function. Just put it here for preserve previous implementation.
	//void SpawnTextInner2D(class ABladeIICharacter* Target, FFloatingAbnormalEffectSetupInfo& EffectCreateInfo, FLoadedFloatingAbnormalEffectSetupInfo& LoadedHolderRef);
	static void EmissiveOverride(class ABladeIICharacter* Target, const FB2MaterialParamData& Data);

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
public:
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};

