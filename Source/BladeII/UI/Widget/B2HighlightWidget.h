// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "B2HighlightWidget.generated.h"

/**
 * 
 */
class UB2UITutorial;

UCLASS()
class BLADEII_API UB2HighlightWidget : public UImage
{
	GENERATED_BODY()

public:
	UB2HighlightWidget(const FObjectInitializer& ObjectInitializer);

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnSlatePaint(const FGeometry& Geometry);

	virtual void SynchronizeProperties() override;
	void ExtractTutorialInfo(const FString& WidgetName, int32& TutorialID, int32& TutorialStep, bool& bDialog);

	void SetActivate(bool IsActivate);
	FORCEINLINE bool IsActivate() const { return bActivate; }

	bool IsGeometryChanged(const FGeometry& PrevDraw, const FGeometry& CurrDraw);

	void TickTutorialCheck();

	FVector2D ConvertPosition(const FGeometry& Geometry);
	FVector2D ConvertSize(const FGeometry& Geometry);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ActiveTutorialID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ActiveTutorialStep;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bDialogWidget;

	FGeometry	CachedGeometry;
	bool		bActivate;

	TWeakObjectPtr<UB2UITutorial> CachedTutorialUI;
};
