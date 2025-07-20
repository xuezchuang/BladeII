// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "B2Button.generated.h"


UENUM(BlueprintType)
enum class EGoodsInfoPopupType : uint8
{
	EGoodsPopup_None = 0,
	EGoodsPopup_Info,
	EGoodsPopup_Shortcut,
	EGoodsPopup_Check,		//info Popup阑 且瘤 Goto Popup 且瘤 八荤窍绊 搬沥窍绰 扑诀
	EGoodsPopup_Check_Shortcut, //Check甫 秦焊绊 何练窍搁 Goto Popup阑 剁匡 巴 牢瘤, 盔贰 扁瓷阑 且 巴牢瘤 搬沥窍绰 扑诀
	EGoods_PopupEnd,
};

UENUM(BlueprintType)
enum class EGoodsButtonType : uint8
{
	EGoodsButtonType_NONE = 0,
	EGoodsButtonType_Info,
	EGoodsButtonType_InfoShortcut,
	EGoodsButtonType_ShortageShortcut,
	EGoods_PopupEnd,
};

/** Variable and utility set for dynamically created on-touch impulse ring. */
USTRUCT()
struct FImpluseRingInstInfo
{
	GENERATED_USTRUCT_BODY()

	FImpluseRingInstInfo()
	{
		WholeAnimTime = 0.2f;
		CurrentAnimTime = 0.0f;
		MIDParamRange_Begin = 0.5f;
		MIDParamRange_End = 1.5f;
		bIsCurrentOn = false;
		ImpulseRingMID = NULL;
	}

private:
	TWeakObjectPtr<UImage> ImpulseRingImage;

	float WholeAnimTime; // Supposed to be copied property
	float CurrentAnimTime; // 0.0 ~ WholeAnimTime

	float MIDParamRange_Begin;
	float MIDParamRange_End;

	uint32 bIsCurrentOn : 1;
	
	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* ImpulseRingMID; // Cached when it is on.

public:
	/** Init with passing some parameters. */
	bool Init(class UB2UnitedWidgetBase* InOwnerUnitedWidget, const FName& InImageWidgetName, float InWholeAnimTime, float InBeginMIDParam, float InEndMIDParam);
	bool Init(class UB2UnitedWidgetBase* InOwnerUnitedWidget, UImage* InImageWidgetPtr, float InWholeAnimTime, float InBeginMIDParam, float InEndMIDParam);
	void Clear();

	void Play();
	void Stop();

	/** It returns true when it finishes one cycle. If bAutoRepeatOnEnd is true, it automatically restart play anyway. */
	bool Tick(float DeltaSecond, bool bAutoRepeatOnEnd = false);
private:
	void ApplyMIDParam();
public:
	/** For dynamic touching impulse of lobby, not for battle UI, which uses statically placed impulse ring. 
	 * InPosUpperLeftOrigin is supposed to be in UMG ref resolution. */
	void SetImageCenterPosition(const FVector2D& InPosUpperLeftOrigin); 
	FORCEINLINE bool IsCurrentOn() const { return bIsCurrentOn; }
};

/**
 * For BladeII custom button features on top of original UBtton.
 */
UCLASS()
class BLADEII_API UB2Button : public UButton
{
	GENERATED_BODY()

	uint32 bIsPressedForImpulseRing : 1; // We need a little bit extra press condition for impulse ring

	uint32 bIsEnabledByNormalUsage : 1; // Same as bIsEnabled in normal circumstances, but it will cache that value for some special case.

	//UPROPERTY(Transient)
	//class UB2UnitedWidgetBase* CachedOwnerUnitedWidget;

protected:
	
	/** Play specific sound registered at CommonSoundInfo when clicked. Negative value will do nothing. */
	UPROPERTY(EditAnywhere, Category = "BladeII Sound")
	int32 DefaultOnClickSoundIndex;

	/** Played at Press event if true, otherwise played at Click event. */
	UPROPERTY(EditAnywhere, Category = "BladeII Sound")
	uint32 bPlayDefaultOnClickSoundOnPress : 1;

	/** Supposed to be an Image widget, special ring effect material assigned. */
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	FName OnTouchImpulseRingWidgetName;
	
	/** The time taken to fully extend the ring by material animation. */
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	float OnTouchImpulseRingAnimTime;
	
	/** Parameter range of impulse ring material animation. */
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	float OnTouchImpulseRingParamRange_Begin;
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	float OnTouchImpulseRingParamRange_End;
	
	/** Impulse ring will be continuously played if button touch/click state is maintained. */
	UPROPERTY(EditAnywhere, Category = "BladeII Sound")
	uint32 bLoopOnTouchImpulseRing : 1;

	FImpluseRingInstInfo OnTouchImpulseRingInst;

	/** How long it will be disabled on other button pressing? 0 or less will be stunned for a single tick. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	float StunDuration;
	float LastStunnedTime;
	/** Stunning is setting a button being disabled to ensure safer button handling (but itself is hacky). 
	 * A button can be disabled with bIsStunned being false, but when bIsStunned is true, the button is disabled. */
	uint32 bIsStunned : 1;

public:
	UB2Button(const FObjectInitializer& ObjectInitializer);

	virtual void SetIsEnabled(bool bInIsEnabled) override;

	//////////////////////////////////////////////////
	// Only for special use. Do NOT use it for any other usage..
	FORCEINLINE bool IsStunned() const {return bIsStunned;}
	FORCEINLINE bool ShouldStunSingleTickOnly() const { return (StunDuration <= 0.0f); }
	void StunForDuration();
	/** Supposed to be called at every frame when it is stunned. Returns true on restore. */
	bool TryRestoreFromStun();
	//////////////////////////////////////////////////

	bool IsTickRequired() const; // Its owning user widget will iterate this button when it returns true;
	void ManualTick(float DeltaSecond);

	virtual void RegisterExtraStuff(class UB2UnitedWidgetBase* InOwnerUnitedWidget);
	virtual void UnregisterExtraStuff(class UB2UnitedWidgetBase* InOwnerUnitedWidget);

	void OnChangedClickMethod();
	
protected:
	UFUNCTION()
	virtual void ExtraOnClick();
	UFUNCTION()
	virtual void ExtraOnPress();
	UFUNCTION()
	virtual void ExtraOnRelease();
	UFUNCTION()
	virtual void ExtraOnHovered();
		
	virtual void PlayOnClickDefaultSound();
	virtual void PlayOnTouchImpulseRing();
	virtual void StopOnTouchImpulseRing();
	virtual void OnOnTouchImpulseRingAnimTimeCycle(); // For every OnTouchImpulseRingAnimTime second.

public:

	virtual void CustomStopOnTouchImpulseRing(); // Even when you call it, the impulse ring will stop after one cycle has passed.
	void TutorialButtonSignal();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
	FORCEINLINE void EditorOnlySetClickSoundIndex(int32 InIndex) { DefaultOnClickSoundIndex = InIndex; }
#endif
	
};
