// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UnitedWidgetBase.h"
#include "B2DebugTextWidget.generated.h"

/** Info struct for queueing new debug text draw */
struct FDebugTextWidgetTextEntry
{
	FDebugTextWidgetTextEntry()
	{
		UpperLeftCoord = FVector2D(0.0f, 0.0f);
		FontColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
		FontSize = 12;
		bShadowed = true;
		ShadowColor = FLinearColor(0.0f, 0.0f, 0.0f);
	}

	FString TheText;
	FVector2D UpperLeftCoord;
	FSlateColor FontColor;
	int32 FontSize;
	bool bShadowed;
	FLinearColor ShadowColor;

	void SetTBByThisInfo(UTextBlock* InTB);
	void SetCPSByThisInfo(UCanvasPanelSlot* InCPS);
};

USTRUCT()
struct FTimedDebugTextWidgetTextEntry
{
	GENERATED_USTRUCT_BODY()

	FTimedDebugTextWidgetTextEntry()
	{
		CurrentDisplayedTime = 0.0f;
		MaxDisplayTime = 0.0f;
		CreatedTB = NULL;
	}

	FDebugTextWidgetTextEntry DebugTextInfo;

	float CurrentDisplayedTime;
	float MaxDisplayTime;

	UPROPERTY(Transient)
	UTextBlock* CreatedTB;
};

/*
 * A full screen sized widget contains dynamically created text blocks displaying various debug information.
 * Must be created only for development build
 */
UCLASS()
class BLADEII_API UB2DebugTextWidget : public UB2UnitedWidgetBase
{
	GENERATED_BODY()

	const int32 MaxTimedListSize = 40;
	const float TimedTextLineHeight = 20.0f;

protected:

	TWeakObjectPtr<UCanvasPanel> CP_Base;

	/** The text list displayed at next update (probably next frame) */
	TArray<FDebugTextWidgetTextEntry> DebugTextQueue;

	UPROPERTY(Transient)
	TArray<UTextBlock*> AllCreatedTBs;

	UPROPERTY(Transient)
	/** This list is maintained and removed at reserved time */
	TArray<FTimedDebugTextWidgetTextEntry> TimedTextList;

public:
	UB2DebugTextWidget(const FObjectInitializer& ObjectInitializer);

	//
	// Most functions here are to be stripped out for real shipping build.. but sometimes need for shipping build so use different define.
	//
#if WITH_BII_ON_SCREEN_DEBUG_TEXT

	static TSubclassOf<UB2DebugTextWidget> GetUniqueClassForScreenDisplay(); // Get the widget blueprint class for whole screen text display.
	
	static int32 GetViewportZOrder();

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void DestroySelf();

	/** The main interface for draw text, intended to be called at each frame. Tried to be similar to FCanvas::DrawShadowedString
	 * It adds text entry by designated info, to be drawn at next update (tick) */
	void EnqueueDrawText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize = 12, bool bDrawShadow = true, const FLinearColor& InShadowColor = FLinearColor::Black);

	/** Draw a text for designated duration, just called once, */
	void AddTimedDrawText(const FString& InText, const FLinearColor& InFontColor, int32 InFontSize = 12, float InDisplayDuration = 2.0f, bool bDrawShadow = true, const FLinearColor& InShadowColor = FLinearColor::Black);

protected:

	void CacheAssets();

	/** Add text entry for next update, possibly for next frame drawing. */
	void AddNewTextEntry(const FDebugTextWidgetTextEntry& NewTextEntry);

	void AddTimedTextEntry(const FTimedDebugTextWidgetTextEntry& NewTimedEntry);

	void UpdateDrawText(); // Destroy exiting text widgets and create by added DebugTextQueue.

	void UpdateTimedText(float InDeltaSecond);

	void DestroyAllTexts();

#endif

};
