#pragma once

#include "EngineMinimal.h"
#include "CommonStruct.h"


FORCEINLINE int32 B2GraphicsLevelToInt(EB2GraphicsLevel InGL) { return static_cast<int32>(InGL); }
FORCEINLINE EB2GraphicsLevel IntToB2GraphicsLevel(int32 InInt) { return static_cast<EB2GraphicsLevel>(InInt); }

FORCEINLINE int32 B2FrameLevelToInt(EB2FrameLimitLevel InFL) { return static_cast<int32>(InFL); }
FORCEINLINE EB2FrameLimitLevel IntToB2FrameLimitLevel(int32 InInt) { return static_cast<EB2FrameLimitLevel>(InInt); }

FORCEINLINE int32 B2ResolutionLevelToInt(EB2ResolutionLevel InRL) { return static_cast<int32>(InRL); }
FORCEINLINE EB2ResolutionLevel IntToB2ResolutionLevel(int32 InInt) { return static_cast<EB2ResolutionLevel>(InInt); }

/** RQ (Resolution Quality) type selection for BladeII game implementation layer.
 * It is separated from EB2GraphicsLevel to provide independent user selection. */
UENUM()
enum class EB2GraphicsRQType : uint8
{
	/**********************************************************
	 [REMARK]
	 Enum entries in higher value has priority of being considered as higher level.
	 See GetResolutionLevelOfRQType and IsResolutionLevelAvailable.
	************************************************************/

	LowerScreenScale_1 = 0,
	LowerScreenScale_2 = 1,

	Default = 2, // No AA or extra scale, maintain the setting specified by scalability system

	MSAAx2 = 3,
	MSAAx4 = 4,

	/** It simply upscale the ScreenPercentage (ResolutionQuality) even more than 100%.
	 * It is not like higher resolution, but provide some sort of Anti-Aliasing. */
	ExtraScreenScale_1 = 5,
	ExtraScreenScale_2 = 6,
	ExtraScreenScale_3 = 7,

	/** MobileContentScaleFactor */
	ExtraContentScale_1 = 8,
	ExtraContentScale_2 = 9,

	/*
	 ====================================
	 For additional identifier.
	 1) Check CVar "r.AllowedGameRQType_H/M/L" 
	 2) Check CVar "r.DefaultB2GraphicsRQType".
	 3) Check DeviceProfiles.ini

	 .. and any other necessary to support the additional feature.
	 ====================================
	 */

	End
};
FORCEINLINE bool IsRQTypeOneOfLowerScreenScale(EB2GraphicsRQType InType) { return (InType == EB2GraphicsRQType::LowerScreenScale_1 || InType == EB2GraphicsRQType::LowerScreenScale_2); }
FORCEINLINE bool IsRQTypeOneOfExtraScreenScale(EB2GraphicsRQType InType) { return (InType == EB2GraphicsRQType::ExtraScreenScale_1 || InType == EB2GraphicsRQType::ExtraScreenScale_2 || InType == EB2GraphicsRQType::ExtraScreenScale_3); }
FORCEINLINE bool IsRQTypeOneOfExtraContentScale(EB2GraphicsRQType InType) { return (InType == EB2GraphicsRQType::ExtraContentScale_1 || InType == EB2GraphicsRQType::ExtraContentScale_2); }
FORCEINLINE bool IsRQTypeOneOfMSAA(EB2GraphicsRQType InType) { return (InType == EB2GraphicsRQType::MSAAx2 || InType == EB2GraphicsRQType::MSAAx4); }

FORCEINLINE EB2GraphicsRQType GetContentScaleRQTypeFallback(EB2GraphicsRQType InType)
{
	// Meant to be used when ExtraContentScale (MCSF) does not properly work.
	return 
		(InType == EB2GraphicsRQType::ExtraContentScale_2) ? EB2GraphicsRQType::ExtraScreenScale_3 : (
		(InType == EB2GraphicsRQType::ExtraContentScale_1) ? EB2GraphicsRQType::ExtraScreenScale_2 :
		InType // Otherwise, just pass as the same type.
		);
}

EB2GraphicsRQType GetDefaultGraphicsRQType();

namespace B2Scalability
{
#if !UE_BUILD_SHIPPING
	extern bool bUseBladeIIScalabilityOverride;
#endif
	/**
	 * Got some use for this..
	 * It means.. need to subtract this amount from the value of scalability quality level count, to get actual maximum level 
	 * -1 due to max num to max index, and the highest level is for "Cine" so subtract -1 again, therefore -2
	 *		->> And now, subtract 1 again, as our deployed scalability uses up to level 2, not 3.
	 */
	const int32 MaxPossibleQualityLevelSubtract = 3;

	void OnModuleStartup();

#if PLATFORM_ANDROID
	void AndroidMCSFUpdateHack(bool bForceInvalidateResCache = false);
#endif

	void OnGameModeStartPlay(class ABladeIIGameMode* StartingGM);
	FORCEINLINE void OnGameModeStartPlay(class AB2DLCFrontGameMode* StartingGM)
	{ // Suffered from mysterious link error when the implemented body is at the cpp.
#if PLATFORM_ANDROID
		AndroidMCSFUpdateHack(true);
#endif
	}
	void OnGameModeLoadingScreenEnd();

	/** 
	 * Returns expected GraphicsLevel as detected by DeviceProfile.
	 * It resets engine scalability level to default detected by DeviceProfile.
	 */
	EB2GraphicsLevel GetGraphicsLevelByExpectedScalability();

	void AdjustScalabilityBySelectedLevel(UObject* WorldContextObject, EB2GraphicsLevel InOverallGraphicsLevel, EB2ResolutionLevel InResolutionLevel, bool bExtraContentScaleFallback = false);

	FORCEINLINE void AdjustScalabilityBySelectedLevel(UObject* WorldContextObject, int32 InOverallGraphicsLevelInt, int32 InResolutionLevelInt, bool bExtraContentScaleFallback = false) {
		return AdjustScalabilityBySelectedLevel(WorldContextObject, IntToB2GraphicsLevel(InOverallGraphicsLevelInt), IntToB2ResolutionLevel(InResolutionLevelInt), bExtraContentScaleFallback);
	}

	void AdjustScalabilityBySavedUserSelection(UObject* WorldContextObject);

	/** Additional scalability control by stage event cinematic scene, called at begin/end of stage event. */
	void ApplyStageEventSpecificScalabilitySetting(UObject* WorldContextObject, bool bIsShowOn);
	
	/** Provide mapping between EB2FrameLimitLevel (for user interface) and actually used internal variable. */
	IConsoleVariable* GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel InRelativeLevel);
	void SetRelativeFrameLimitLevel(EB2FrameLimitLevel InRelativeLevel);
	EB2FrameLimitLevel GetCurrentRelativeFrameLimitLevel();
	EB2FrameLimitLevel GetDefaultRelativeFrameLimitLevel();
	bool IsRelativeFrameLimitLevelAvailable(EB2FrameLimitLevel InCheckLevel);

	/** Provide mapping between EB2ResolutionLevel (for user interface) and actually used internal variable. */
	IConsoleVariable* GetMappedCVarOfResolutionLevel(EB2ResolutionLevel InLevel);
	EB2GraphicsRQType GetMappedRQTypeOfResolutionLevel(EB2ResolutionLevel InLevel);
	EB2GraphicsRQType GetSafeMappedRQTypeOfResolutionLevel(EB2ResolutionLevel InLevel);
	EB2ResolutionLevel GetResolutionLevelOfRQType(EB2GraphicsRQType InRQType);
	bool IsResolutionLevelAvailable(EB2ResolutionLevel InCheckLevel);
	bool DoesRQTypeRequiresAppRestart(EB2GraphicsRQType InCheckRQType);

	/** Master reset function of all Bladeii scalability feature. */
	void ResetAllScalabilitySettings();
}

//float GetExtraAppliedCameraDistance(float InBaseLength, float InExtraLength);