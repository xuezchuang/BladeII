// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Images/SImage.h"

/**
 * 
 */
class BLADEII_API STickImage : public SImage
{
public:
	STickImage()
	{
		//bCanTick = true;
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SetOwnerWidget(class UB2HighlightWidget* Owner)
	{
		//OwnerWidget = Owner;
	}
	
	class UB2HighlightWidget* OwnerWidget;
};
