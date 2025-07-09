// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2DynTextBase.h"
#include "B2DynText_Multiple.generated.h"

DECLARE_DELEGATE_TwoParams(UB2DynText_Multiple_OnClickSelectBtn, int32, FVector2D);
DECLARE_DELEGATE(UB2DynText_Multiple_OnClickInfoBtn);
/**
 * A single text row which can be used to display text in multiple text block in dynamic manner.
 * Design is up to its derived BP widget class.
 */
UCLASS()
class BLADEII_API UB2DynText_Multiple : public UB2DynTextBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FLinearColor ModColor_SelectedSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FLinearColor ModColor_EmptyValue;

protected:
	TWeakObjectPtr<UImage> IMG_IndexIcon;
	TWeakObjectPtr<UTextBlock> TB_1;
	TWeakObjectPtr<UTextBlock> TB_2;
	TWeakObjectPtr<UTextBlock> TB_3;
	TWeakObjectPtr<UB2Button> BTN_Select;

	TWeakObjectPtr<UB2Button> BTN_OptionInfo;
	//
	// You can add more TB_#, with additional SetDynText method having additional argument.
	//

public:
	UB2DynText_Multiple(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CacheAssets() override;

public:
	// Use according to its needs.
	void SetDynText(int32 InRowIndex, const FText& InText1);
	void SetDynText(int32 InRowIndex, const FText& InText1, const FSlateColor& TextColor);
	void SetDynText(int32 InRowIndex, const FText& InText1, const FText& InText2);
	void SetDynText(int32 InRowIndex, const FText& InText1, const FText& InText2, const FText& InText3);
	void SetDynText(int32 InRowIndex, const FText& InText1, const FText& InText2, const FText& InText3, const FSlateColor& TextColor);
	void SetDynTextColor(int32 TextBlock, const FSlateColor& TextColor);
	void SetVisibility_BtnSelect(bool IsVisible);
	void SetChangeSelectButtonColor();
	void SetChangeEmptyValueColor(int32 TextBlock);
	void SetChangeDefaultSelectButtonColor();
	void SetVisibility_Textbox(int32 TextNumber, ESlateVisibility Visible);
	void SetVisibleOptionButton(bool bVisible);
	void SetIndexIcon(int32 InRowIndex);
public:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnClickedBTNSelect();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnClick_BtnOption();
public:
	UB2DynText_Multiple_OnClickSelectBtn FClickSelectBtn;
	UB2DynText_Multiple_OnClickInfoBtn FClickOptionInfoBtn;
private:
	int32 RowIndex;
};
