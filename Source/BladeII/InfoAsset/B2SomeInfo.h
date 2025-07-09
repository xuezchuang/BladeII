// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "UObject/Object.h"
#include "BladeII.h"
#include "UObject/ObjectSaveContext.h"
#include "Engine/StreamableManager.h"
#include "CommonStruct.h"
#include "B2SomeInfo.generated.h"

/**
 * Asset container for any info assets which can be hardly categorized by specific standard.
 * Put "some" stuff here.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2SomeInfo : public UObject
{
	GENERATED_BODY()

	FStreamableManager InfoLoadManager;

public:
	UB2SomeInfo(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> StageClearWingFxTemplate;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedStageClearWingFxTemplate; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetStageClearWingFxTemplate();
	void UnloadStageClearWingFxTemplate();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> StageClearGemFxTemplate;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedStageClearGemFxTemplate; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetStageClearGemFxTemplate();
	void UnloadStageClearGemFxTemplate();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> StageClearBGFxTemplate;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedStageClearBGFxTemplate; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetStageClearBGFxTemplate();
	void UnloadStageClearBGFxTemplate();

protected:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class ABladeIIPlayer> DefaultPCClassInfoBaseBPClass; // It might better goes to PCClassInfo..
	UPROPERTY(Transient)
	TSubclassOf<class ABladeIIPlayer> LoadedDefaultPCClassInfoBaseBPClass; // Loaded hard ptr for preventing GC.
public:
	TSubclassOf<class ABladeIIPlayer> GetDefaultPCClassInfoBaseBPClass();
	void UnloadDefaultPCClassInfoBaseBPClass();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directing")
	class UForceFeedbackEffect* ForceFeedbackSetting;
public:
	UForceFeedbackEffect* GetForceFeedbackEffect() {
		return ForceFeedbackSetting;
	}

protected:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class ABladeIICharacter> DefaultNPCClassInfoBaseBPClass; // It might better goes to NPCClassInfo..
	UPROPERTY(Transient)
	TSubclassOf<class ABladeIICharacter> LoadedDefaultNPCClassInfoBaseBPClass; // Loaded hard ptr for preventing GC.
public:
	TSubclassOf<class ABladeIICharacter> GetDefaultNPCClassInfoBaseBPClass();
	void UnloadDefaultNPCClassInfoBaseBPClass();

protected:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class AB2DropItem> DefaultDropItemBaseBPClass;
	UPROPERTY(Transient)
	TSubclassOf<class AB2DropItem> LoadedDefaultDropItemBaseBPClass; // Loaded hard ptr for preventing GC.
public:
	TSubclassOf<class AB2DropItem> GetDefaultDropItemBaseBPClass();
	void UnloadDefaultDropItemBaseBPClass();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UBehaviorTree> DefaultNPCClassInfoMeleeBT; // It might better goes to NPCClassInfo..
	UPROPERTY(Transient)
	class UBehaviorTree* LoadedDefaultNPCClassInfoMeleeBT; // Loaded hard ptr for preventing GC.
public:
	class UBehaviorTree* GetDefaultNPCClassInfoMeleeBT();
	void UnloadDefaultNPCClassInfoMeleeBT();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UBehaviorTree> MountAutoBT;
	UPROPERTY(Transient)
	class UBehaviorTree* LoadedMountAutoBT;
public:
	class UBehaviorTree* GetMountAutoBT();
	void UnloadMountAutoBT();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UBehaviorTree> DefaultNPCClassInfoSpawnBT; // It might better goes to NPCClassInfo..
	UPROPERTY(Transient)
	class UBehaviorTree* LoadedDefaultNPCClassInfoSpawnBT; // Loaded hard ptr for preventing GC.
public:
	class UBehaviorTree* GetDefaultNPCClassInfoSpawnBT();
	void UnloadDefaultNPCClassInfoSpawnBT();


protected:
	// Extra attached by PC mesh composition system. Six star effect is defined by ItemInfo.
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level20;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level20; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level20();
	void UnloadWeaponExtraEffect_Level20();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level30;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level30; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level30();
	void UnloadWeaponExtraEffect_Level30();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level40_Gl;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level40_Gl; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level40_Gl();
	void UnloadWeaponExtraEffect_Level40_Gl();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level40_As;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level40_As; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level40_As();
	void UnloadWeaponExtraEffect_Level40_As();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level40_Fi;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level40_Fi; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level40_Fi();
	void UnloadWeaponExtraEffect_Level40_Fi();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level40_Wz;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level40_Wz; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level40_Wz();
	void UnloadWeaponExtraEffect_Level40_Wz();

	UPROPERTY(EditAnywhere)
	int32 UseWeaponExtraEffect_Level40;

	int32 GetUseWeaponExtraEffect_Level40();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level50_Gl;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level50_Gl; // Loaded hard ptr for preventing GC.

public:
	class UParticleSystem* GetWeaponExtraEffect_Level50_Gl();
	void UnloadWeaponExtraEffect_Level50_Gl();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level50_As;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level50_As; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level50_As();
	void UnloadWeaponExtraEffect_Level50_As();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level50_Fi;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level50_Fi; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level50_Fi();
	void UnloadWeaponExtraEffect_Level50_Fi();

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> WeaponExtraEffect_Level50_Wz;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedWeaponExtraEffect_Level50_Wz; // Loaded hard ptr for preventing GC.
public:
	class UParticleSystem* GetWeaponExtraEffect_Level50_Wz();
	void UnloadWeaponExtraEffect_Level50_Wz();

	UPROPERTY(EditAnywhere)
	int32 UseWeaponExtraEffect_Level50;

	int32 GetUseWeaponExtraEffect_Level50();

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> SurviveSkillRestoreEffect;
	UPROPERTY(Transient)
	class UParticleSystem* LoadedSurviveSkillRestoreEffect; // Loaded hard ptr for preventing GC.

public:
	class UParticleSystem* GetSurviveSkillRestoreEffect();
	void UnloadSurviveSkillRestoreEffect();

protected:
	/** Each element index matches casted int of EPCClass enum. */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> PCIntroFlipbookMtrls;
	/** Element index for this loaded array does NOT match to EPCClass enum */
	UPROPERTY(Transient)
	TArray<class UMaterialInterface*> LoadedPCIntroFlipbookMtrls; // Loaded hard ptr for preventing GC.
public:
	class UMaterialInterface* GetPCIntroFlipbookMtrl(EPCClass InPCClass);
	void UnloadAllPCIntroFlipbookMtrl(); // Unload for all PC classes


	//
	// To be added..
	//



protected:
	/** This list is for use "just once" at first time game execution, for shader caching purpose, then this list will be forgotten.
	 * No loaded list caching */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UParticleSystem>> UniquePreRenderParticleSystems;
public:
	void LoadAndConsumeUniquePreRenderPS(TArray<class UParticleSystem*>& OutLoaded);
protected:
	/** Another list for use "just once" at first time game execution, for shader caching purpose.
	 * Pre-rendering of NPCClass will be done without this kind of unique list, when a game-level using the NPCClass is loaded at first time.
	 * This list is provided for doing it in advance at PreRenderGameMode.
	 * Only specailly chosen classes will be here, not all. */
	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<class UB2NPCSingleClassInfo>> UniquePreRenderNPCClass;
public:
	void LoadAndConsumeUniquePreRenderNPCClass(TArray<TSubclassOf<class UB2NPCSingleClassInfo>>& OutLoaded);
protected:
	/** This list is for use "just once" at first time game execution, for shader caching purpose, then this list will be forgotten.
	 * No loaded list caching */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class USkeletalMesh>> UniquePreRenderSkMeshes;
public:
	void LoadAndConsumeUniquePreRenderSKMesh(TArray<class USkeletalMesh*>& OutLoaded);


	/** Call this whenever a stage is about to be unloaded */
	void UnloadAll();
#if WITH_EDITOR
	/** For safe use of TSoftClassPtr in editor. */
	void EditorLoadAll();
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP // Clean-up loaded TAsset references. They are temporary code until formally fix wrong handling of lazy-loaded info asset.
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

};
