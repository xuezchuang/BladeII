#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "CommonStruct.h"
#include "BladeIICharacter.h"
#include "../BladeII/BladeIIPlayer.h"
#include "B2PreRenderer.generated.h"

/** Info struct to pre-rendering character and their relevant effects stuff for mobile shader compile */
USTRUCT()
struct FB2GMPreRenderDummyInfo
{
	GENERATED_USTRUCT_BODY();

	FB2GMPreRenderDummyInfo()
	{
		PreRenderDummy = nullptr;
		PreRenderedCount = 0;
	}

	/** It is not really meant to be used for caching, but it is just a proper place to put this, as it is needed per merged mesh. */
	UPROPERTY(Transient)
	class ASkeletalMeshActor* PreRenderDummy;

	/** Dummy will be hidden when this count reaches at MaxPreRenderCount. */
	int32 PreRenderedCount;

	/** Check done for normal condition and update some components..
	 * Returns true if my pre-render is all done. */
	bool UpdateDummy(bool bForceContinue = false);
};

enum class EPreRenderPhase : uint8
{
	EPRP_None,
	EPRP_Selected, // When rendering for specially gathered objects.. originally intended main phase
	EPRP_AllPrim, // A bit extreme handling.. but looks like it make things better..
	EPRP_Done
};

/** Just a combined data struct. PrimitveComponent has two visibility related flags.. */
USTRUCT()
struct FPreRenderPrimCompVisibilityCache
{
	GENERATED_USTRUCT_BODY()
	
	FPreRenderPrimCompVisibilityCache()
		: bTotalVisible(true), bVisible(true), bHiddenInGame(false)
	{}

	bool bTotalVisible;
	bool bVisible;
	bool bHiddenInGame;
};

/** Tracking pre-render state per related object type, for doing pre-render just once if not necessary anymore. */
USTRUCT()
struct FPreRenderCompleteState
{
	GENERATED_USTRUCT_BODY()

	FPreRenderCompleteState()
	{
		bPreRenderLevelGoneThrough = false;
		bDamageEffectInfoPreRendered = false;
	}

protected:
	/** Required only once for each character type. */
	TArray<EPCClass> PreRenderedPCs;

	/** It was part of PCClass data, but now separated, and can be increased by player level up during play.
	 * Particle notifies in anim sequences are the main interest here for pre-render.
	 * TMap just for quick search. Bool key value is supposed to be true, or just to be ignored. */
	TMap<FCombinedPCSkillAnimID, bool> PreRenderedSkillAnims;

	/** TMap just for quick search. Bool key value is supposed to be true, or just to be ignored. */
	TMap<FCombinedNPCClassID, bool> PreRenderedNPCs;

	/** TMap just for quick search. Bool key value is supposed to be true, or just to be ignored. */
	TMap<FName, bool> PreRenderedLevels;

	/** Whether Pre-render level gone through can be checked by PreRenderedLevels state,
	 * but as it has its own unique circumstance, we provide a flag for it. */
	bool bPreRenderLevelGoneThrough;

	bool bDamageEffectInfoPreRendered;
public:
	void MarkPCClassPreRendered(EPCClass InClass);
	bool IsPCClassPreRendered(EPCClass InClass) const;
	
	void MarkSkillAnimPreRendered(FCombinedPCSkillAnimID InCombinedPCSkillAnimId);
	bool IsSkillAnimPreRendered(FCombinedPCSkillAnimID InCombinedPCSkillAnimId) const;

	void MarkNPCClassPreRendered(ENPCClass InClass, ENPCClassVariation InClassVari);
	void MarkNPCClassPreRendered(FCombinedNPCClassID InCombinedNPCId);
	bool IsNPCClassPreRendered(ENPCClass InClass, ENPCClassVariation InClassVari) const;
	bool IsNPCClassPreRendered(FCombinedNPCClassID InCombinedNPCId) const;

	void MarkWorldLevelPreRendered(class UWorld* InWorld);
	bool IsWorldLevelPreRendered(class UWorld* InWorld) const;

	FORCEINLINE void MarkPreRenderLevelGoneThrough() { bPreRenderLevelGoneThrough = true; }
	FORCEINLINE bool IsPreRenderLevelGoneThrough() const { return bPreRenderLevelGoneThrough; }

	FORCEINLINE void MarkDamageEffectInfoPreRendered() { bDamageEffectInfoPreRendered = true; }
	FORCEINLINE bool IsDamageEffectInfoPreRendered() const { return bDamageEffectInfoPreRendered; }

public:
	bool CanSavePreRenderState() const;
	/** Load/Save into local ini. */
	void SaveState();
	void LoadState();
	void ClearState();
private:
	void SavePCClassPreRenderState();
	void LoadPCClassPreRenderState();
	
	void SaveSkillAnimPreRenderState();
	void LoadSkillAnimPreRenderState();

	void SaveNPCClassPreRenderState();
	void LoadNPCClassPreRenderState();

	void SaveWorldLevelPreRenderState();
	void LoadWorldLevelPreRenderState();
};

/** Any info useful for pre-render state traking of a material. */
struct FPreRenderPerMaterialState
{
	FPreRenderPerMaterialState()
	{}

private:
	/** Mobility can affect the actual shader */
	TArray<TEnumAsByte<EComponentMobility::Type>> AllUsedMobilities;

	/** We might add light type used for pre-render the material, as it affects the actual shader type,
	 * but we conrol per-light type shader in different way. */

public:
	void AddUsedMobility(EComponentMobility::Type InMobility);
	bool CheckMobilityUsed(EComponentMobility::Type InMobility) const;
};

/** 
 * Material based pre-render complete state tracking info 
 * Pre-render is essentially material business, so all pre-render complete state better be material basis.
 * However, for practical reason yet. This material based tracking is only limited..
 */
USTRUCT()
struct FPreRenderMaterialCompleteState
{
	GENERATED_USTRUCT_BODY()

	FPreRenderMaterialCompleteState()
	{}

private:
	TMap<FName, FPreRenderPerMaterialState> AllStates;
	const FName GetMaterialKeyName(class UMaterialInterface* InCheckMtrl) const;
public:
	void MarkPrimCompMaterials(class UPrimitiveComponent* InPrimComp);
	bool ArePrimCompAllMaterialsPreRendered(class UPrimitiveComponent* InPrimComp) const;

	void ClearState();
};

/**
 * An object class manages BladeII pre-render operation for caching shaders before game play.
 */
UCLASS()
class BLADEII_API UB2PreRenderer : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	class ABladeIIGameMode* OwnerGameMode;

	/** Main pre-render dummy data array. */
	UPROPERTY(Transient)
	TArray<FB2GMPreRenderDummyInfo> AllPreRenderDummy;

	/** Cached actor and primcomp visibility map for PreRenderAllPrimitives option. */
	UPROPERTY(Transient)
	TMap<UPrimitiveComponent*, FPreRenderPrimCompVisibilityCache> PreRenderAllPrimVisbilityCache;
	
	UPROPERTY(Transient)
	TMap<AActor*, bool> PreRenderAllActorVisbilityCache;

	/** Turn off dynamic pointlights at all-prim phase start and cache the state to restore it when complete. */
	UPROPERTY(Transient)
	TMap<UPointLightComponent*, bool> PreRenderAllPLStateCache;

	/** It statically stores all pre-rendered state throughout the game, to check if specific type of pre-rendering is really necessary. */
	static FPreRenderCompleteState TotalCompleteState;

	/** It does not completely replace TotalComplete state. Just assist it. */
	static FPreRenderMaterialCompleteState MaterialBasedCompleteState;

	/** Tuning Local Player Character point light to make pre-render more effective. */
	uint32 bLightEnvironmentSetupForDynPointLight : 1;
	uint32 bLocalPlayerHeadLightModified : 1;
	float LastCachedLocalPCLightRadius;
	float LastCachedLocalPCLightIntensity;

	UPROPERTY(Transient)
	class APointLight* PreRenderDummyPointLight;

	uint32 bLightEnvironmentSetupForDynDirLight : 1;
	UPROPERTY(Transient)
	class ADirectionalLight* PreRenderDirLight;

	uint32 bHadDoneAnyPreRender : 1;

	uint32 bPendingForPreRenderGM : 1;

	/** Cached when consuming SomeInfo list. */
	UPROPERTY(Transient)
	TArray<UParticleSystem*> CachedExtraPreRenderPS;
	UPROPERTY(Transient)
	TArray<TSubclassOf<class UB2NPCSingleClassInfo>> CachedExtraPreRenderNPCClassInfo;
	UPROPERTY(Transient)
	TArray<USkeletalMesh*> CachedExtraPreRenderSKMesh;

	//UPROPERTY(Transient)
	//class UB2PreLoadingScreen* CachedPreLoadingScreen;
	int32 EstimatedTotalProgressStep; // Being set when SetupPreRenderObjects
	int32 CurrPrerenderProgressStep; // 0 ~ EstimatedTotalProgressStep
	int32 ProgressStepCountForSelectedPhase; // Counted only during selected phase.

public:
	UB2PreRenderer(const FObjectInitializer& ObjectInitializer);
	
	/** The main entry function of Pre-render dummy setup */
	bool SetupPreRenderObjects(class ABladeIIGameMode* InGM);
private:
	// Sub routines of SetupPreRenderObjects, which can be duplicated.
	void ConditionalSetupPreRenderObjectForPCClass(EPCClass InClass, bool bForceSetup = false);
	void ConditionalSetupPreRenderObjectForNPCClass(const FCombinedNPCClassID InClassID, bool bForceSetup = false);
	void ConditionalSetupPreRenderObjectForPCSkillAnims(const TArray<FCombinedPCSkillAnimID>& InSkillIDs, bool bForceSetup = false);
	void ConditionalSetupPreRenderObjectForDamageEffectInfo(bool bForceSetup = false);
	void PostPreRenderSeupProcess();
	
	void SetupPreRenderObjects_FirstLobby();

	void SetupPreRenderObjects_ExtraFx();
	void SetupPreRenderObjects_ExtraNPCClass();
	void SetupPreRenderObjects_ExtraOthers(); // Extra of extra..?
public:	
	///////////////////////////////////////////
	// Pre-render Light environment setup.
	// It's necessary because shader generation is mainly directed by lighting type.

	void ConditionalSetupPreRenderLightEnvForDynPL(bool bForceSetup = false);
	void ConditionalRestoreFromPreRenderLightEnvForDynPL(bool bRestoreAll = true);
	void MinimizePlayerPointLight();
	void ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Type LightMobility, bool bForceSetup = false, bool bSetupForCSM = false);
	void ConditionalRestoreFromPreRenderLightEnvForDirL();
private:
	class ADirectionalLight* FindDirectionalLighOfMobility(EComponentMobility::Type LightMobility);
public:
	void CompletelyTurnOffAllLights();

	///////////////////////////////////////////

	/** Internal functionalities of SetupPreRenderObjects */
	static class ASkeletalMeshActor* SpawnPreRenderMeshCommon(UWorld* InSpawnWorld);
	static FVector GetPreRenderDummyDesiredLocation(UObject* WorldContextObject);
	static class UParticleSystemComponent* SpawnDummyPSCForPreRender(class UParticleSystem* InPS, class USceneComponent* InRootComp);

protected:
	FB2GMPreRenderDummyInfo& GetExtraPrerenderDummyInfo();
	class ASkeletalMeshActor* GetExtraPrerenderDummySKActor();

	FB2GMPreRenderDummyInfo& GetAdditionalPrerenderDummyInfo();
	class ASkeletalMeshActor* GetAdditionalPrerenderDummySKActor();

	void SetupDummyParticleSystemsForPreRender(class ASkeletalMeshActor* InDummySKActor);
	void SetupDummyMobForPreRender(ENPCClass InNPCClass, ENPCClassVariation InVari, class ABladeIICharacter* InOwnerChar, class ASkeletalMeshActor* InDummySKActorForRender);
	/** Need to merge meshes for pre-render dummy too. */
	void SetupDummyPCMeshForPreRender(EPCClass InCharClass, class ABladeIIPlayer* InOwnerPlayer, class ASkeletalMeshActor* InDummySKActorForRender, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData);

public:
	void UpdateForPreRenderObjects(bool bForceContinue = false); // Update Tick

private:
	void SetupDefaultPreRenderScreen();
	void StepPreRenderScreenProgress();
public:
	bool ShouldRenderForAllPrimPhase(class UPrimitiveComponent* InCompToCheck) const;
	bool SetupPreRenderAllPrimPhase();
	/** Returns true on finish */
	bool CheckAndFinishPreRenderAllPrimPhase();

	void OnPreRenderComplete();
	void EnsureSaveCaches();
	
	void MarkCurrentWorldLevelPreRendered();
	bool IsCurrentWorldLevelPreRendered() const;

	static void MarkPreRenderLevelGoneThrough();
	static bool IsPreRenderLevelGoneThrough();
	
	/** True return means it is still actively working. False return does not mean it's all done though. We got AllPrim phase and even multiple passes if currently in PreRenderGameMode. */
	FORCEINLINE bool HasPreRenderDummyToProcess() const { return (AllPreRenderDummy.Num() > 0); }
	/** True means a new specially prepared level will be loaded, even you've just arrived at lobby. You will experience the first lobby after a little while then. */
	FORCEINLINE bool IsPendingForPreRenderGM() const { return bPendingForPreRenderGM; }

	//////////////////////////////
	// Setup methods for PreRenderGameMode

	bool IsInPreRenderGameMode() const;
	void InitForPreRenderGameMode(class AB2PreRenderGameMode* InGM);
	// Setup~~ for each EPRGMContentTypePass type
	void SetupForPreRenderGM_PCClassPass(EPCClass InPCClass, bool bIncludeSkillAnims = false);
	void SetupForPreRenderGM_PCSkillAnimPass(EPCClass InPCClass);
	void SetupForPreRenderGM_DamageEffectInfo();
	void SetupForPreRenderGM_Extra(bool bForNPCClass, bool bForFx, bool bForOther);

#if PLATFORM_IOS // iOS specific pre-render configuration.. pre-render can consume much memory.
	static bool ShouldDoPreRenderForIOS();
	/** Sub-conditions of ShouldDoPreRenderForIOS */
	static bool ShouldDoPreRenderForIOS_Basic();
	static bool ShouldDoPreRenderForIOS_AllPrim();
	static bool ShouldDoPreRenderForIOS_PreRenderGM();
#endif

	//////////////////////////////

	//
	// Configurations and states for Pre-render
	//

	/** How many ticks to pre-render?
	 * In most cases, no need to set it bigger than 1. Just 1 for on and 0 for off.. */
	static int32 MaxPreRenderCount;

	/** All pre-render dummies are to be destroyed after this tick, even if it is examined to not being rendered.
	 * but it is still being counted after at least one dummy is rendered */
	static int32 OverallPreRenderLimit;
	bool bCanDoOverallCount;
	int32 OverallTickCount; // To be compared with OverallPreRenderLimitt..

	/** For the final safety */
	static int32 PreRenderHardLimit;
	int32 TickCountFromSetup;

	static float PreRenderSpawnDistFromView;
	/** If true, pre-render location is far from starting point, to prevent player see it, then no screen is needed. */
	static bool bPreRenderAtFarAway;

	/** Most extreme handling.. */
	static bool bPreRenderAllPrimitives;
	static int32 PreRenderAllPrimLit2PassTickLimit; // Tick limit for 2 Dynamic point lights
	static int32 PreRenderAllPrimLit1PassTickLimit; // Tick limit for 1 Dynamic point light
	static int32 PreRenderAllPrimUnlitPassTickLimit;
	FORCEINLINE static int32 GetTotalPreRenderAllPrimTickLimit() {return PreRenderAllPrimLit2PassTickLimit + PreRenderAllPrimLit1PassTickLimit + PreRenderAllPrimUnlitPassTickLimit;}
	int32 PreRenderAllPrimTickCount;

	static void LoadSettings();
	static void LoadSettings_PreRenderGM();

	static void SaveCompleteState();
	static void LoadCompleteState();
	static void ClearCompleteState();
};