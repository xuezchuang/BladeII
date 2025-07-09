// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "UMG.h"
#include "B2UnitedWidgetBase.h"
#include "B2UIEnum.h"
#include "Event.h"
#include "B2UIWidgetBase.generated.h"

/** Information struct to define any custom timing of a user widget */
USTRUCT(BlueprintType)
struct FB2UIWidgetCustomTimedCallbackInfo
{
	GENERATED_USTRUCT_BODY()

		FB2UIWidgetCustomTimedCallbackInfo()
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
struct FB2UIWidgetCustomTimedHideInfo
{
	GENERATED_USTRUCT_BODY()

		FB2UIWidgetCustomTimedHideInfo()
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


/*
* The base class of all of new BladeII UI system, being managed by B2UIManager. We still got old UI yet, being called as DJLegacy.
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIWidgetBase : public UB2UnitedWidgetBase
{
	GENERATED_BODY()

public:
	/** Define any custom time to call CustomTimeCallback using this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<FB2UIWidgetCustomTimedCallbackInfo> CustomTimedCallBackInfo;

	/** Define custom time to temporarily hide any child widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<FB2UIWidgetCustomTimedHideInfo> CustomTimedHideInfo;

public:
	UB2UIWidgetBase(const FObjectInitializer& ObjectInitializer);
	~UB2UIWidgetBase();

	virtual void Init();

	virtual void NativeConstruct() override;
	
	/** Before you use DestroySelf directly, consider CloseUI or UIManager first. */
	virtual void DestroySelf(class UB2UIManager* InUIManager);

#if WITH_EDITOR
	virtual void PIEEndDestroy(class UB2UIManager* InUIManager);
#endif //WITH_EDITOR


public:
	/** It gives the opportunity to set the text Localized in BP. */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void UpdateStaticText_BP();

protected:

	/** Prevent BladeII UserWidget (UB2UIWidgetBase), which is not bound to specific level, being destroyed on world change. */
	//-> It now being destroyed with world. We found some problems. Better not let them static.
	virtual bool DestroyOnLevelRemove() /*override*/ { return true; }

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
	
	/*Where the handling in order to use the asset on the BP in NativeCode.*/
	virtual void CacheAssets(){}

	/*Allows setting the content of the text does not change here.*/
	virtual void UpdateStaticText(){}

	/* Where to bind a DelegateFunction to cached assets. */
	virtual void BindDelegates(){};

	virtual bool OnBackButtonProc() { return false; }
};

