// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIPowerValueChange.h"


void UB2UIPowerValueChange::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UCanvasPanel, C_StatUp);
	GET_SLOT(UTextBlock, TB_CountUp);
	GET_SLOT(UCanvasPanel, C_StatDown);
	GET_SLOT(UTextBlock, TB_CountDown);
	GET_SLOT(UWidgetAnimation, Anim_StatUp);
	GET_SLOT(UWidgetAnimation, Anim_StatDown);
}

void UB2UIPowerValueChange::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIPowerValueChange::Init()
{
	Super::Init();
}

void UB2UIPowerValueChange::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}

void UB2UIPowerValueChange::SetValueAndPlay(int32 ChangeAmount)
{
	if (ChangeAmount > 0)
	{
		if (TB_CountUp.IsValid())
			TB_CountUp->SetText(FText::FromString(FString::FromInt(ChangeAmount)));
		if (C_StatUp.IsValid())
			C_StatUp->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (C_StatDown.IsValid())
			C_StatDown->SetVisibility(ESlateVisibility::Collapsed);
		//Anim_StatUp->PlayAnimation();
		//Anim_StatDown->StopAnimation();
	}
	else
	{
		if (TB_CountDown.IsValid())
			TB_CountDown->SetText(FText::FromString(FString::FromInt(ChangeAmount)));
		if (C_StatDown.IsValid())
			C_StatDown->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (C_StatUp.IsValid())
			C_StatUp->SetVisibility(ESlateVisibility::Collapsed);
		//Anim_StatDown->PlayAnimation();
		//Anim_StatUp->StopAnimation();
	}
}

void UB2UIPowerValueChange::SetValueOnlyImage(bool bIsUp)
{
	if (TB_CountUp.IsValid())
		TB_CountUp->SetVisibility(ESlateVisibility::Collapsed);
	if (TB_CountDown.IsValid())
		TB_CountDown->SetVisibility(ESlateVisibility::Collapsed);

	if (bIsUp)
	{
		if (C_StatUp.IsValid())
			C_StatUp->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (C_StatDown.IsValid())
			C_StatDown->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (C_StatDown.IsValid())
			C_StatDown->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (C_StatUp.IsValid())
			C_StatUp->SetVisibility(ESlateVisibility::Collapsed);
	}
}

