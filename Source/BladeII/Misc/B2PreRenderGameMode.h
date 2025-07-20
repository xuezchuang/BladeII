// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "BladeIIGameMode.h"
#include "B2PreRenderGameMode.generated.h"

/** Too many kind of lighting environment might be just waste.. */
#define _PRE_RENDER_GM_MORE_LIGHT_PASS 0
#define _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY 1

/**
 * A special GameMode for optimization purpose, supposed to be played only once after game installation. 
 * It executes pre-rendering of resource objects not bound to levels.
 */
UCLASS()
class BLADEII_API AB2PreRenderGameMode : public ABladeIIGameMode
{
	GENERATED_BODY()

protected:
	
	////////////////////////////////////////////////////
	// Identifiers for pre-render state to be executed here 

	/** The most outer pass. The whole pre-rendering here will be executed per light environment setup */
	enum class EPRGMLightPass : uint8
	{
		LP_None,

		LP_StationaryDirectionalLight, // including 2 dynamic point lights.

#if _PRE_RENDER_GM_MORE_LIGHT_PASS && !_PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
		LP_MovableDirectionalLight,
#endif
		LP_End
	};
	EPRGMLightPass CurrentLightPass;
	FORCEINLINE EPRGMLightPass GetFirstLightPass() const { return (EPRGMLightPass)((int32)EPRGMLightPass::LP_None + 1); }
	FORCEINLINE bool IsLastLightPass(EPRGMLightPass InLightPass) const
	{
		return InLightPass == (EPRGMLightPass)((int32)EPRGMLightPass::LP_End - 1);
	}

	/** Each content type has its own pre-render pass, for safe pre-render execution with less possibility of hurting the rendering hardware capability. */
	enum class EPRGMContentTypePass : uint8
	{
		CTP_None,
		
		CTP_DamageEffectInfo, // it is lighter than PC, so better be the first one..?

		CTP_PC, // PC pass will be separated internally by each character type.
		CTP_PC_SkillAnim, // Like basic PC pass, it will be separated by each character type.

		// NPCs might be too much to do all at here. Let them be done when each level does its own pre-render.

		CTP_Extra_NPC,

		CTP_Extra_PC_NPC_CSM, // For CacadedShadowMap handling.. instead of another LightPass, somewhat like CTP_Extra_MovableDirLight

		CTP_Extra_FX,

#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
		/** Instead of general LP_MovableDirectionalLight pass, try movable directional light only for some of extra content type. */
		CTP_Extra_MovableDirLight,
#endif

		CTP_End
	};
	EPRGMContentTypePass CurrentContentTypePass;
	FORCEINLINE EPRGMContentTypePass GetFirstContentTypePass() const { return (EPRGMContentTypePass)((int32)EPRGMContentTypePass::CTP_None + 1); }
	FORCEINLINE bool IsLastContentTypePass(EPRGMContentTypePass InContentTypePass) const
	{
		return InContentTypePass == (EPRGMContentTypePass)((int32)EPRGMContentTypePass::CTP_End - 1);
	}

	EPCClass CurrentPCClassPass;
	FORCEINLINE EPCClass GetFirstPCClassPass() const { return IntToPCClass(0); }
	FORCEINLINE bool IsLastPCClassPass(EPCClass InPCClassPass) const
	{
		// Considering possibility that GetMaxPCClassNum and EPC_End can be different, EPC_End looks better in this case.
		return InPCClassPass == IntToPCClass(PCClassToInt(EPCClass::EPC_End) - 1);
	}

	float PreRenderTimeScale;
	float SinglePassLeastTime;
	float TimeSinceLastPassBegin;

	/** Each pass again,, has separated lit pass.. */
	int32 InternalLit2PassTickLimit;
	int32 InternalLit1PassTickLimit;
	int32 CurrentPassTickCount;

	float WeightedPassSumSoFar; // For progress tracking
	FORCEINLINE int32 GetTotalLightPassNum() const { return ((int32)EPRGMLightPass::LP_End - 1); }
	int32 GetTotalContentTypePassNum() const;
	float GetTotalWeightedPassNum() const;
	// First light pass has more weight on progress because of info asset loading.
	float GetFistLightPassProgressWeight() const { return 2.5f; }

public:
	AB2PreRenderGameMode(const FObjectInitializer& ObjectInitializer);

	// Might better have its own identification..?
	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::PreRender; }

protected:
	/** Called before any other actor's BeginPlay is called. */
	virtual void PreBeginPlay() override;
	virtual void BeginPlay() override;

	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;

	virtual void SetupNextGameLoadingScreen() override;

	virtual void Tick(float DeltaSecond) override;

	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad() override;

	// PreRender interfaces from the parent (BladeIIGameMode)
	virtual void SetupPreRenderObjects() override;
	virtual bool IsInPreRenderPhase() const override;
	virtual void UpdateForPreRenderObjects() override;

	// PreRender pass management.. specifically for this GameMode.
	void ProceedToNextPass();
	void SetupForLightPass(EPRGMLightPass InSetupLightPass);
	void SetupForContentTypePass(EPRGMContentTypePass InContentTypePass);
	void SetupForPCClassPass(EPCClass InPCClass); // Sub pass of ContentTypePass CTP_PC
	void SetupForPCSkillAnimPass(EPCClass InPCClass); // Sub pass of ContentTypePass CTP_PC_SkillAnim
	void SetupForExtraCSMPass();
	void SetupForExtraFxAndOtherPass();
#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
	void SetupForMovableLightExtraOnlyPass();
#endif

	bool ShouldDoExtraCSMPass() const;
	bool ShouldDoExtraFxPass() const;
#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
	bool ShouldDoMovableLightExtraOnlyPass() const;
#endif

	void OnCompleteAll();

	void UpdateProgressDisplay();
};

bool CheckBladeIIPreRenderGameAvailability();
bool StartBladeIIPreRenderGame(class AGameMode* InCurrentGameMode);