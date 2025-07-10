// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
////#include "BladeII.h"
#include "UMG.h"
#include "B2UnitedWidgetBase.h"
#include "B2UIWidgetBase.h"
#include "BladeIIUserWidget.generated.h"

/** Information struct to define any custom timing of a user widget */
USTRUCT(BlueprintType)
struct FWidgetCustomTimedCallbackInfo
{
	GENERATED_USTRUCT_BODY()

	FWidgetCustomTimedCallbackInfo()
	{
		InvokeTime = 0.0f;

		bIsInvoked = false;
	}

	/** Time (in second) to call the callback after the construction of owning user widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InvokeTime;

	FTimerHandle TimerHandle;

	/** Special flag introduced to use the custom timer when basic timer is not available. 
	 * Don't worry about this if basic timer is operational. */
	uint32 bIsInvoked : 1;
};

/** Information struct to define any custom timing to hide child widgets */
USTRUCT(BlueprintType)
struct FWidgetCustomTimedHideInfo
{
	GENERATED_USTRUCT_BODY()

	FWidgetCustomTimedHideInfo()
	{
		HideStartTime = 0.0f;
		HideStopTime = 0.0f;
		WidgetNameToHide = NAME_None;
		OriginalVisibility = ESlateVisibility::Visible;

		bIsHidden = false;
		bHiddenDoneOnce = false;
	}

	/** Time (in second) to start hide after the construction of owning user widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HideStartTime;

	/** Time (in second) to make it visible after the construction of owning user widget. 
	 * The exact visibility state is what was originally set before hidden (usually what you set at UMG) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HideStopTime;

	/** Name of the widget that can be seen in the UMG. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WidgetNameToHide;

	/** Cached before hide, to restore it back. */
	ESlateVisibility OriginalVisibility;

	FTimerHandle HideStartTimerHandle;
	FTimerHandle HideStopTimerHandle;

	/** Current hidden state */
	uint32 bIsHidden : 1;

	uint32 bHiddenDoneOnce : 1; // For manual timer.
};

/** For special purpose Particle system property set to create 3D Fx under specific location of slate widget. */
USTRUCT(BlueprintType)
struct FSingleUMGParticleSetup
{
	GENERATED_USTRUCT_BODY()

	FSingleUMGParticleSetup()
	{
		CreateDepth = 10.0f;
		FxTemplate = NULL;
		FxScale = FVector(1.0f, 1.0f, 1.0f);
	}
	/** Used for computing final 3D coordinate of creating Fx.
	 * Better not use too big value for proper screen space alignment. You can still adjust scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	float CreateDepth;

	/** The particle system template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* FxTemplate;

	/** Created component's scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector FxScale;

	class UParticleSystemComponent* CreateUnderScreen(APlayerController* InPC, FVector2D ScreenPos);
};


/**
 * Base of UserWidget for BladeIIGame, only for DJLegacy now,,
 */
UCLASS()
class BLADEII_API UBladeIIUserWidget : public UB2UnitedWidgetBase
{
	GENERATED_BODY()


protected:

	/** Allows blurring on background rendered image using DOF. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldBackgroundBlur")
	uint32 bAllowWorldBackgroundBlur : 1;

	/** Transient flag for if using WorldBackgroundBlur now. */
	uint32 bWorldBackgroundBlurOn : 1;

	/** How much blur the WorldBackgroundBlur will be in general? 
	 * -> It does not really working for mobile. Looks like due to performance consideration. Just a dummy variable for now.. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldBackgroundBlur")
	float WorldBackgroundBlurScale;

	/** It is rather a special flag for WorldBackgroundBlur, as DOF cannot blur the UI.
	 * InGameHUD is generally expected to not be required when WorldBackgroundBlur is used. 
	 * If a widget need to hide in-game HUD in any other reason, find other way. This is only for the WorldBackgroundBlur */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldBackgroundBlur")
	uint32 bHideInGameHUDForWorldBackgroundBlur : 1;

	/** Define any custom time to call CustomTimeCallback using this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FWidgetCustomTimedCallbackInfo> CustomTimedCallBackInfo;

	/** Define custom time to temporarily hide any child widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FWidgetCustomTimedHideInfo> CustomTimedHideInfo;

public:
	
	UBladeIIUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
protected:
	/** The place where you can cache various placed widgets to native references */
	virtual void CacheAssets() {}

private:
	/** Start WorldBackgroundBlur if bAllowWorldBackgroundBlur is true. It can fail in some circumstances, then it returns false. */
	bool StartWorldBackgroundBlur();
	/** Stop WorldBackgroundBlur, automatically called when it is destroyed. */
	void StopWorldBackgroundBlur();

protected:

	/** Setup for CustomTimedCallBackInfo. Override if the widget is used under paused circumstance. */
	virtual void SetupWidgetCustomTimer();

	/** Main callback to be invoked by CustomTimedCallBackInfo */
	void OriginalCustomTimeCallback(int32 InfoIndex);
	/** For any native CustomTimeCallback override at subclass. */
	virtual void NativeCustomTimeCallback(int32 InfoIndex);
	/** Blueprint callback to be invoked by CustomTimedCallBackInfo */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void CustomTimeCallback(int32 InfoIndex);


	/** Setup for CustomTimedHideInfo. Override if the widget is used under paused circumstance. */
	virtual void SetupWidgetCustomHideTimer();
	/** Main callback to be invoked by CustomTimedHideInfo */
	void CustomTimeHideStartCallback(int32 InfoIndex);
	void CustomTimeHideStopCallback(int32 InfoIndex);

public:

	virtual void DestroySelf();
};
