// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UITalkPortrait.h"

void UB2UITalkPortrait::CacheAssets()
{
	GET_SLOT(UImage, IMG_Portrait);
	GET_SLOT(UImage, IMG_NameBG);
	GET_SLOT(UB2RichTextBlock, RTB_Name);
}

void UB2UITalkPortrait::BindDelegates()
{
	
}

void UB2UITalkPortrait::SetReversePortrait(bool IsReverse)
{
	FVector2D Dir = IsReverse ? FVector2D(-1.0f, 1.0f) : FVector2D(1.0f, 1.0f);

	if (RTB_Name.IsValid())
		RTB_Name->SetRenderScale(Dir);
}

void UB2UITalkPortrait::SetVisibleName()
{
	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (IMG_NameBG.IsValid())
		IMG_NameBG->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (RTB_Name.IsValid())
		RTB_Name->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UB2UITalkPortrait::SetVisible(bool IsVisible)
{
	bIsVisible = IsVisible;
	ESlateVisibility VisibleState = IsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
	
	this->SetVisibility(VisibleState);

 	if (IMG_Portrait.IsValid())
 		IMG_Portrait->SetVisibility(VisibleState);
 
 	if (IMG_NameBG.IsValid())
 		IMG_NameBG->SetVisibility(VisibleState);
 
 	if (RTB_Name.IsValid())
 		RTB_Name->SetVisibility(VisibleState);

	if (IsVisible)
		this->SetColorAndOpacity(FLinearColor::White);
}

bool UB2UITalkPortrait::GetVisible()
{
	return bIsVisible;
}

void UB2UITalkPortrait::SetPortraitImage(UTexture2D *Texture)
{
	if (IMG_Portrait.IsValid())
		IMG_Portrait->SetBrushFromTexture(Texture);
}

void UB2UITalkPortrait::SetSpeakerName(const FText &Name)
{
	if (RTB_Name.IsValid())
		RTB_Name->SetText(Name);
}

void UB2UITalkPortrait::SetActivate()
{
	OnActivate_BP(true);
}

void UB2UITalkPortrait::SetInactivate()
{
	OnActivate_BP(false);
}