// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __B2GAMELOADINGSCREEN_H__
#define __B2GAMELOADINGSCREEN_H__

//#include <ModuleInterface.h>
#include "MoviePlayer.h"
#include "SlateBasics.h"

DEFINE_LOG_CATEGORY_STATIC(LogB2GameLoadingScreen, Log, All);

// Mirrors FMovieSubtitle...
struct B2MovieSubtitle
{
	FText Text;
	FSlateColor Color;
	float StartTime;
	float EndTime;
};

enum class EB2LoadingMessageType : uint8
{
	ELMT_None,
	ELMT_GameLoading,
	ELMT_LobbyLoginLoading,

	ELMT_End // For counting
};

/** Return true for unusual screen aspect ratio such as iPAD. */
B2GAMELOADINGSCREEN_API bool ShouldUseShorterScreenLoadingImageLayout(const FVector2D& CurrViewportSize);

/** Brining some functions implemented at BladeII module. */
typedef bool(*LoadGameSettingFloatFnType)(float& OutValue);
extern B2GAMELOADINGSCREEN_API LoadGameSettingFloatFnType gLoadGameFxVolumeSettingFnPtr;
extern B2GAMELOADINGSCREEN_API LoadGameSettingFloatFnType gLoadGameBGMVolumeSettingFnPtr;

/** Module interface for this game's loading screens */
class IB2GameLoadingScreenModule : public IModuleInterface
{
public:

	virtual void StartGameLoadingScreen_Image(const TCHAR* LoadingImageName, 
		bool bInDisplayProgress, bool bInDisplayIndicator = false,
		const FVector2D InViewSize = FVector2D(1280.0f,720.0f), 
		bool bFullyStretchImage = false,
		float InOptimalAspectRatio = 2.055556f, // Required only when bFullyStretchImage is false
		EB2LoadingMessageType InMessageType = EB2LoadingMessageType::ELMT_GameLoading
	) = 0;

	///** It returns subtitle info struct, where you can set proper text and other attributes from outside. */
	//virtual TArray<B2MovieSubtitle>* StartGameLoadingScreen_Movie(const TCHAR* MovieName, 
	//	// Most default values are suit to scenario movie play.
	//	ELoadingScreenMovieAlignMode InAlignmentMode = ELoadingScreenMovieAlignMode::FitToMin,
	//	bool bInEnableMovieSkipButton = true, 
	//	bool bInDisplayProgressBar = false,
	//	bool bInDisplayIndicator = false,
	//	EMoviePlaybackType InMoviePlayBackType = EMoviePlaybackType::MT_Normal, 
	//	bool bAutoCloseOnLoadingComplete = false
	//	) = 0;

	static FName GetModuleName() { return FName(TEXT("B2GameLoadingScreen")); }
};

FORCEINLINE IB2GameLoadingScreenModule* GetB2LoadingScreenModulePtr() { return FModuleManager::LoadModulePtr<IB2GameLoadingScreenModule>(IB2GameLoadingScreenModule::GetModuleName()); }

/** 
 * Simplified FProgressAnimateUtil of BladeII module.
 * It is duplicated here.. as it is somewhat isolated module. 
 * but not so critical even if it gets different.
 */
struct FLoadingProgressBarAnim
{
	float BaseSpeed; 
	float SpringConst;
	float DampConst;

private:
	float TargetPercentScale; // 0.0 ~ 1.0 scale
	float CurrentAnimatedPercentScale; // The result progress percent value, 0.0 ~ 1.0.

	float CurrentSpeed;
	
public:
	FLoadingProgressBarAnim();

	void Tick(float InDeltaSeconds);

	void SetTargetValue(float InTargetPercentScale /*0.0 ~ 1.0*/, bool bSyncCurrentValues = false);

	void SyncToTargetValue(); // It force finish progress animation to target value.

	float GetUnitDistanceToTargetValue(); // How far the progress should go to reach the target? 0.0 Means reached at target.
	
	FORCEINLINE float GetCurrentAnimatedPercentScale() const { return CurrentAnimatedPercentScale; }
};


#endif // __SHOOTERGAMELOADINGSCREEN_H__
