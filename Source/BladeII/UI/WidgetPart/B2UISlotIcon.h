// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UISlotIcon.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSlotIconClickDelgate, UB2UISlotIcon*, ClickedSlot);

/*
* In the form of slots to draw the image.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISlotIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;

	UB2UISlotIcon(const FObjectInitializer& ObjectInitializer);

	void SetIcon(UTexture2D* Image);
	void SetIcon(UMaterialInterface* InMaterial);
	void SetIcon(FString ImagePath);

	UMaterialInterface* GetIconMaterial() const;

	void ShowIcon(const bool bShow);


	//To enable the button operation, and uses for the case that only the Disable icon.
	void SetIconDisable(const bool bDisabled);

	void SetClickable(const bool bEnable) { bClickable = bEnable; }
	bool GetClickable()	{ return bClickable; }

	// For being child as element of Scroll-Box;
	void SetPreciseClickAndTab(bool bToSetPrecise);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//=================================================== Btn
	UFUNCTION()
		virtual void OnClickBtnArea();

public:
	FOnSlotIconClickDelgate OnSlotIconClickDelgate;

	UPROPERTY(EditAnywhere, Category = "BladeII SlotIcon")
		UMaterialInterface* DefaultImage;


protected:
	TWeakObjectPtr<UB2Button>		BTN_Area;
	TWeakObjectPtr<UImage>		IMG_Icon;

private:
	bool bClickable;
	EButtonClickMethod::Type DefaultClickMethod;
	EButtonTouchMethod::Type DefaultTouchMethod;
};
