// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynText_Multiple.h"
//#include "BladeII.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"



UB2DynText_Multiple::UB2DynText_Multiple(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RowIndex = -1;
	ModColor_SelectedSlot = FLinearColor::Blue;
	ModColor_EmptyValue = FLinearColor(0.15f, 0.15f, 0.15f);
}

void UB2DynText_Multiple::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_IndexIcon);
	GET_SLOT(UTextBlock, TB_1);
	GET_SLOT(UTextBlock, TB_2);
	GET_SLOT(UTextBlock, TB_3);
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UB2Button, BTN_OptionInfo);

	if (BTN_OptionInfo.IsValid())
		BTN_OptionInfo->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2DynText_Multiple::SetDynText(int32 InRowIndex, const FText& InText1)
{
	RowIndex = InRowIndex;

	ModulateBGImageToRowIndex(InRowIndex); // 줄 구분 용.

	if (TB_1.IsValid())
		TB_1->SetText(InText1);
}

void UB2DynText_Multiple::SetDynText(int32 InRowIndex, const FText& InText1, const FText& InText2)
{
	SetDynText(InRowIndex, InText1);

	if (TB_2.IsValid())
		TB_2->SetText(InText2);
}

void UB2DynText_Multiple::SetDynText(int32 InRowIndex, const FText& InText1, const FText& InText2, const FText& InText3)
{
	SetDynText(InRowIndex, InText1, InText2);

	if (TB_3.IsValid())
		TB_3->SetText(InText3);
}

void UB2DynText_Multiple::SetDynText(int32 InRowIndex, const FText& InText1, const FSlateColor& TextColor)
{
	if (TB_1.IsValid())
		TB_1->SetColorAndOpacity(TextColor);

	SetDynText(InRowIndex, InText1);
}

void UB2DynText_Multiple::SetDynText(int32 InRowIndex, const FText& InText1, const FText& InText2, const FText& InText3, const FSlateColor& TextColor)
{
	SetDynText(InRowIndex, InText1, InText2);

	if (TB_3.IsValid())
		TB_3->SetText(InText3);
	if (TB_3.IsValid())
		TB_3->SetColorAndOpacity(TextColor);
}

void UB2DynText_Multiple::SetDynTextColor(int32 TextBlock, const FSlateColor& TextColor)
{
	if (TextBlock == 0 && TB_1.IsValid())
		TB_1->SetColorAndOpacity(TextColor);
	
	else if (TextBlock == 1 && TB_2.IsValid())
		TB_2->SetColorAndOpacity(TextColor);

	else if (TextBlock == 2 && TB_3.IsValid())
		TB_3->SetColorAndOpacity(TextColor);
}

void UB2DynText_Multiple::SetVisibility_BtnSelect(bool IsVisible)
{
	if (BTN_Select.IsValid())
		BTN_Select->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2DynText_Multiple::SetChangeSelectButtonColor()
{
	if (IMG_BG.IsValid())
		IMG_BG->SetColorAndOpacity(ModColor_SelectedSlot);
}

void UB2DynText_Multiple::SetChangeEmptyValueColor(int32 TextBlock)
{
	if (TextBlock == 0 && TB_1.IsValid())
		TB_1->SetColorAndOpacity(ModColor_EmptyValue);

	else if (TextBlock == 1 && TB_2.IsValid())
		TB_2->SetColorAndOpacity(ModColor_EmptyValue);

	else if (TextBlock == 2 && TB_3.IsValid())
		TB_3->SetColorAndOpacity(ModColor_EmptyValue);
}

void UB2DynText_Multiple::SetChangeDefaultSelectButtonColor()
{
	ModulateBGImageToRowIndex(RowIndex);
}

void UB2DynText_Multiple::SetVisibility_Textbox(int32 TextNumber, ESlateVisibility Visible)
{
	switch (TextNumber)
	{
	case 0:
	{
		if (TB_1.IsValid())
			TB_1->SetVisibility(Visible);
	}
	break;
	case 1:
	{	if (TB_2.IsValid())
			TB_2->SetVisibility(Visible);
	}
	break;
	case 2:
	{	if (TB_3.IsValid())
			TB_3->SetVisibility(Visible);
	}
	break;
	}
}

void UB2DynText_Multiple::OnClickedBTNSelect()
{
	FVector2D WidgetPos = GetWidgetScreenPos(this);
	WidgetPos.X += GetDesiredSize().X;
	FClickSelectBtn.ExecuteIfBound(RowIndex, WidgetPos);
}


void UB2DynText_Multiple::OnClick_BtnOption()
{
	FClickOptionInfoBtn.ExecuteIfBound();
}

void UB2DynText_Multiple::SetVisibleOptionButton(bool bVisible)
{
	if (BTN_OptionInfo.IsValid())
		BTN_OptionInfo->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UB2DynText_Multiple::SetIndexIcon(int32 InRowIndex)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_IndexIcon.IsValid())
	{
		int32 IconIndex = InRowIndex + 1;
		IMG_IndexIcon->SetBrushFromMaterial(ItemMiscInfo->GetTotemOptionIndexIcon(IconIndex));
	}
}