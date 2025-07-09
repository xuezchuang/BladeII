#pragma once

#include "BladeIIUserWidget.h"
#include "BladeIIGameMode.h"
#include "B2PreLoadingScreen.generated.h"

const int32 PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM = 5;

struct FPreRenderProgressTextState
{
	FPreRenderProgressTextState()
	{
		bAnimatedTextInSeparatedWidget = true;
		bDisplayProgressText = false;
		CurrentProgressScale = 0.0f;
		CurrentAnimateState = 0;
		TimeSinceLastAnimUpdate = 0.0f;
	}

	// Belows are combined to make CurrentResultText..

	FText BasicInfoText;
	
	/** If true, CurrentAnimateState is not used to directly change the result string, instead it is used to turn on/off separate text widget. */
	bool bAnimatedTextInSeparatedWidget;

	bool bDisplayProgressText;
	float CurrentProgressScale; // Must be set from outside.
private:
	int32 CurrentAnimateState;
	float TimeSinceLastAnimUpdate;
	
	static float AnimatePeriod;
	
public:
	void Tick(float DeltaSecond);
	void ResetAnimState();
	FText GetCurrentResultText();
	FORCEINLINE int32 GetCurrentAnimateState() const { return CurrentAnimateState; }
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2PreLoadingScreen : public UBladeIIUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> DefaultLoadingImagePtr;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> DefaultPreRenderImagePtr;

	/** To be rotated */
	int32 FirstPreRenderImageIndex;
	/** We will try to keep this interval, but each (blocked) pre-render phase took so long, so actual interval would be longer.. */
	UPROPERTY(EditAnywhere)
	float FirstPreRenderImageRotateInterval;
	double LastFPRImageRotateTickTime;
	double TimeSinceFPRImageRotate;

	TWeakObjectPtr<UImage> Image_BG;
	TWeakObjectPtr<UTextBlock> TB_BottomText;
	TWeakObjectPtr<UTextBlock> TB_PreRenderInfo;
	TArray<TWeakObjectPtr<UTextBlock>> TB_PreRenderInfo_AnimPieces;

	TWeakObjectPtr<UProgressBar> PB_PreRenderProgress;

	TWeakObjectPtr<UTextBlock> TB_MiddleText1;
	TWeakObjectPtr<UTextBlock> TB_MiddleText2;
	
	TWeakObjectPtr<UPanelWidget> P_LoadingWheel;

	EPreLoadingScreenType PreLoadingType;

	bool bShouldDisplayPreRenderText;
	FPreRenderProgressTextState PreRenderProgressTextState;
	bool bDisplayPreRenderProgressBar;

	FProgressAnimateUtil PreRenderPBAnimatedValue;

public:

	UB2PreLoadingScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void CacheAssets() override;

	virtual void DestroySelf() override;

	void SetPreLoadingType(EPreLoadingScreenType InType, UTexture2D* InSpecifiedImage);
	void SetLoadingText_Bottom(const FText& InText);
	void SetLoadingText_Middle1(const FText& InText);
	void SetLoadingText_Middle2(const FText& InText);

	void SetupPreRenderProgress(bool bIsForFistPreRenderGM, bool bDisplayProgressText, bool bDisplayProgressBar);
	void UpdatePreRenderProgressScale(float InNewProgressScale, bool bSyncProgressImmediately = false); // 0.0 ~ 1.0
	void ApplyCurrentPreRenderProgressText();
	void ApplyCurrentPreRenderProgressBar();

private:
	void UpdateForFirstPreRenderImage(float InDeltaTime);
	UTexture2D* GetCurrentFirsPreRenderImage();
};