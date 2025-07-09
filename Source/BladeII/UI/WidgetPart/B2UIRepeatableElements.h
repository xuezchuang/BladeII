// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIRepeatableElements.generated.h"

/*
* A widget that easily handles the elements that repeat like the stars.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRepeatableElements : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;

	UB2UIRepeatableElements(const FObjectInitializer& ObjectInitializer);

	void SetMaxElementNum(int32 InValue);
	void SetCurElementNum(int32 InValue);

protected:
	virtual void CacheAssets() override;

private:
	void ShowFilledWidgets(int32 Num);
	void ShowEmptyWidgets(bool bShow);

public:
	/* Filled only image display and, Empty image hides. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RepeatableElements")
		bool bHideEmptyImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RepeatableElements")
		int32 MaxNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RepeatableElements")
		int32 CurNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RepeatableElements")
		FString PrefixFilledAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RepeatableElements")
		FString PrefixEmptyAsset;


protected:
	/* Each element must be made to the Panel. Because it must contain the image Filled with Empty image.
	 * Filled Image should use the name of the PrefixFilledAsset.
	 * Empty Image should use the name of the PrefixEmptyAsset.*/
	TWeakObjectPtr<UPanelWidget>			CON_Elements;

	
	TArray<UWidget*>			FilledWidgets;
	TArray<UWidget*>			EmptyWidgets;
};
